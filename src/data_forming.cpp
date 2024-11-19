#include "../include/header.h"

// Find the groundtruth for the datasets of SIGMOD contest with brute force, for k = 100
void find_store_groundtruth(string dataset_f, string query_f, string groundtruth_f) {

    // Read the dataset and the queries
    auto r = read_sigmod_dataset(dataset_f); auto r2 = read_sigmod_queries(query_f);
    Dataset dataset = r.first; vector<int> C = r.second;
    Dataset queries = r2.first; vector<int> V = r2.second;

    // Write the groundtruth to "data/SIGMOD/1M_DS/contest-groundtruth-release-1m.bin"
    ofstream file(groundtruth_f, ios::binary);
    if (!file) {
        cerr << "Error, can not open the file data/SIGMOD/1M_DS/contest-groundtruth-release-1m.bin" << endl;
        exit(1);
    }

    // Write the number of queries
    uint32_t num_u = (uint32_t)queries.size();
    file.write(reinterpret_cast<char*>(&num_u), sizeof(uint32_t));

    // Find the groundtruth for each query
    for (int i = 0; i < (int)queries.size(); i++) {
        Data query = queries[i];
        int V_i = V[i];
        
        // Find the distances to all the vectors
        vector<pair<int, double>> distances;
        for (size_t j = 0; j < dataset.size(); j++) {
            if (V_i != -1 && C[j] != V_i) continue;
            double dist = euclidean_distance(query, dataset[j]);
            distances.emplace_back((int)j, dist);
        }

        // Sort the distances
        sort(distances.begin(), distances.end(), [](const pair<int, double> &a, const pair<int, double> &b) {
            return a.second < b.second;
        });

        // Store the k nearest neighbours (or less)
        vector<int> neighbours;
        for (size_t j = 0; j < distances.size() && j < 100; j++) {
            neighbours.push_back(distances[j].first);
        }

        // Write the number of neighbours
        int num_neighbours = neighbours.size();
        file.write(reinterpret_cast<char*>(&num_neighbours), sizeof(int));

        // Write the neighbours
        file.write(reinterpret_cast<char*>(neighbours.data()), 100 * sizeof(int));
    }
}

// Read the groundtruth from the SIGMOD contest (Dataset: data, vector<int> attributes)
vector<vector<int>> read_sigmod_groundtruth(string file_name_s) {
    char* file_name = &file_name_s[0];
    auto *file = fopen(file_name, "r");
    if (file == nullptr) {
        cout << "Error, can not open the file " << file_name << endl;
        return {};
    }

    // Reading the number of the vectors
    uint32_t num_u;
    size_t r = fread(&num_u, sizeof(uint32_t), (size_t)1, file);
    if (r != (size_t)1) {
        cerr << "Error_01, can not read the file " << file_name << endl;
        exit(1);
    }
    int num = (int)num_u;

    // Loop to read every vector
    vector<vector<int>> groundtruth; groundtruth.reserve(num);
    for (int i = 0; i < num; i++) {
        // Read the number of neighbours
        int num_neighbours;
        r = fread(&num_neighbours, sizeof(int), (size_t)1, file);
        if (r != (size_t)1) {
            cerr << "Error_02, can not read the file " << file_name << endl;
            exit(1);
        }

        // Read the neighbours
        vector<int> neighbours(num_neighbours);
        r = fread(neighbours.data(), sizeof(int), (size_t)num_neighbours, file);
        if (r != (size_t)num_neighbours) {
            cerr << "Error_03, can not read the file " << file_name << endl;
            exit(1);
        }
        // Store the neighbours
        groundtruth.push_back(neighbours);
    }

    fclose(file);

    return groundtruth;
}

// Read queries from the SIGMOD contest (Dataset: data, vector<int> attributes)
pair<Dataset, vector<int>> read_sigmod_queries(string file_name_s) {
    // Open the file
    char* file_name = &file_name_s[0];
    auto *file = fopen(file_name, "r");
    if (file == nullptr) {
        cout << "Error, can not open the file " << file_name << endl;
        return {};
    }

    // Reading the number of the vectors
    uint32_t num_u;
    size_t r = fread(&num_u, sizeof(uint32_t), (size_t)1, file);
    if (r != (size_t)1) {
        cerr << "Error_01, can not read the file " << file_name << endl;
        exit(1);
    }
    int num = (int)num_u;

    // Vectors have 104 dimensions (float32): the first is query_type, the second is V
    Dataset arr; arr.reserve(num);
    vector<int> V_arr; V_arr.reserve(num);

    // Loop to read every vector
    for (int i = 0; i < num; i++) {

        // Read the whole vector
        vector<float> v(104);
        r = fread(v.data(), sizeof(float), (size_t)104, file);
        if (r != (size_t)104) {
            cerr << "Error_02, can not read the file " << file_name << endl;
            exit(1);
        }

        // Store the target categorical attribute
        int V = (int)v[1];

        // Store the query type and V
        int query_type = (int)v[0];

        //// If we search for only the timestamp, look only for the ANN
        if (query_type == 2) continue;//query_type = 0;

        //// If we search for both timestamp and categorical attribute, look only for the categorical attribute
        if (query_type == 3) continue;//query_type = 1;

        // Keep only the data
        vector<data_t> v2(v.begin() + 4, v.end());

        // Store the vector and the filter
        arr.push_back(v2);
        V_arr.push_back(V);
    }

    fclose(file);

    // Return the dataset and the filters
    return make_pair(arr, V_arr);
}

// Reading the dataset from the SIGMOD contest
pair<Dataset, vector<int>> read_sigmod_dataset(string file_name_s) {

    // Open the file
    char* file_name = &file_name_s[0];
    auto *file = fopen(file_name, "r");
    if (file == nullptr) {
        cout << "Error, can not open the file " << file_name << endl;
        return {};
    }

    // Reading the number of the vectors
    uint32_t num_u;
    size_t r = fread(&num_u, sizeof(uint32_t), (size_t)1, file);
    if (r != (size_t)1) {
        cerr << "Error_01, can not read the file " << file_name << endl;
        exit(1);
    }
    int num = (int)num_u;

    // Each vector has 102 dimensions (float32):  C, T, 100 features
    Dataset arr; arr.reserve(num);
    vector<int> filters; filters.reserve(num);

    // Loop to read every vector
    for (int i = 0; i < num; i++) {

        // Read the whole vector
        vector<float> v(102);
        r = fread(v.data(), sizeof(float), (size_t)102, file);
        if (r != (size_t)102) {
            cerr << "Error_02, can not read the file " << file_name << endl;
            exit(1);
        }
        
        int C = (int)v[0];
        
        vector<data_t> v2(v.begin() + 2, v.end());

        // Store the vector and the filter
        arr.push_back(v2);
        filters.push_back(C);
    }

    fclose(file);

    // Return the dataset and the filters
    return make_pair(arr, filters);
}

// Reading vectors from files of .bvecs format
Dataset bvecs_read(string file_name_s){
    char* file_name = &file_name_s[0];
    auto *file = fopen(file_name, "r");
    if (file == nullptr) {
        cout << "Error, can not open the file " << file_name << endl;
        return {};
    }

    //Reading the size of the vectors
    int num;
    size_t r = fread(&num, sizeof(int), (size_t)1, file);
    if (r != (size_t)1) {
        cout << "Error_01, can not read the file " << file_name << endl;
        return {};
    }
    
    int vecsizeof = 4 + num;
    
    fseek(file, 0, SEEK_END);

    // Calculate the number of vectors
    long veccount = ftell(file) / vecsizeof;
    
    Dataset arr; arr.reserve(veccount);
    fseek(file, 0, SEEK_SET);

    //We have a loop to read every vector
    for (int i = 0; i < veccount; i++) {
        fseek(file, sizeof(int), SEEK_CUR);
        vector<uint8_t> v(num);
        r = fread(v.data(), sizeof(uint8_t), (size_t)num, file);
        vector<data_t> v2(v.begin(), v.end());
        if (r != (size_t)num) {
            cout << "Error_02, can not read the file " << file_name << endl;
            return {};
        }
        arr.push_back(v2);
    }
    fclose(file);

    return arr;
}

// Reading vectors from files of .fvecs format
Dataset fvecs_read(string file_name_s){
    char* file_name = &file_name_s[0];
    auto *file = fopen(file_name, "r");
    if (file == nullptr) {
        cout << "Error, can not open the file " << file_name << endl;
        return {};
    }

    //Reading the size of the vectors
    int num;
    size_t r = fread(&num, sizeof(int), (size_t)1, file);
    if (r != (size_t)1) {
        cout << "Error_01, can not read the file " << file_name << endl;
        return {};
    }
    
    int vecsizeof = 4 + num*4;
    
    fseek(file, 0, SEEK_END);

    // Calculate the number of vectors
    long veccount = ftell(file) / vecsizeof;
    Dataset arr; arr.reserve(veccount);
    fseek(file, 0, SEEK_SET);

    //We have a loop to read every vector
    for (int i = 0; i < veccount; i++) {
        fseek(file, sizeof(int), SEEK_CUR);
        vector<float> v(num);
        r = fread(v.data(), sizeof(float), (size_t)num, file);
        vector<data_t> v2(v.begin(), v.end());

        if (r != (size_t)num) {
            cout << "Error_02, can not read the file " << file_name << endl;
            return {};
        }
        
        arr.push_back(v2);
    }
    fclose(file);

    return arr;
}

// Reading vectors from files of .ivecs format
vector<vector<int>> ivecs_read(string Filename_s) {
    char* Filename = &Filename_s[0];
    auto fid = fopen(Filename,"r");

    if (fid == NULL) {
        cout << "I/O error : Unable to open the file "<< Filename << endl;
        return {};
    }

    int num;
    size_t r = fread(&num, sizeof(int), (size_t)1, fid);
    if (r != (size_t)1) {
        cout << "Error_01, can not read the file " << Filename << endl;
        return {};
    }

    int vecsizeof = 4 + num * 4;

    fseek(fid , 0, SEEK_END);

    long vecnum = ftell(fid) / vecsizeof;
    vector<vector<int>> arr; arr.reserve(vecnum);

    fseek(fid, 0, SEEK_SET);

    for(int i = 0; i < vecnum; i++) {
        fseek(fid, sizeof(int), SEEK_CUR);
        vector<int> v(num);
        r = fread(v.data(), sizeof(int), (size_t)num, fid);
        if (r != (size_t)num) {
            cout << "Error_02, can not read the file " << Filename << endl;
            return {};
        }
        arr.push_back(v);
    }
    fclose(fid);

    return arr;
}
