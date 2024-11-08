#include "../include/header.h"

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

// Storing the graph in a binary file
void store_graph(const Graph &G, string filename) {
    
    // Open the file
    ofstream file(filename, ios::binary);
    if (!file) return;

    // Write the number of nodes
    int num_nodes = (int)G.size();
    file.write(reinterpret_cast<char*>(&num_nodes), sizeof(int));

    // Write the data and the out neighbours of each node
    for (const auto &node : G) {
        // Write the data
        int dim = (int)node->data.size();
        file.write(reinterpret_cast<char*>(&dim), sizeof(int));
        file.write(reinterpret_cast<char*>(node->data.data()), dim * sizeof(data_t));

        // Write the number of out neighbours
        int num_neighbours = (int)node->out_neighbours.size();
        file.write(reinterpret_cast<char*>(&num_neighbours), sizeof(int));

        // Write the out neighbours indices
        for (auto neighbour : node->out_neighbours) {
            file.write(reinterpret_cast<char*>(&neighbour), sizeof(int));
        }
    }

    file.close();

    return;
}

// Reading the graph from a binary file
Graph read_graph(string filename) {
    Graph G;
    ifstream file(filename, ios::binary);
    if (!file) {
        return G;
    }

    // Read the number of nodes
    int num_nodes;
    file.read(reinterpret_cast<char*>(&num_nodes), sizeof(int));

    // Read the data and the out neighbours of each node
    for (int i = 0; i < num_nodes; i++) {
        // Read the data
        int dim;
        file.read(reinterpret_cast<char*>(&dim), sizeof(int));
        Data data(dim);
        file.read(reinterpret_cast<char*>(data.data()), dim * sizeof(data_t));

        // Create the node
        Graph_Node node = create_graph_node(data);

        // Read the index of the node
        node->indx = i;

        // Read the number of out neighbours
        int num_neighbours;
        file.read(reinterpret_cast<char*>(&num_neighbours), sizeof(int));

        // Read the out neighbours indices
        for (int j = 0; j < num_neighbours; j++) {
            int neighbour;
            file.read(reinterpret_cast<char*>(&neighbour), sizeof(int));
            node->out_neighbours.insert(neighbour);
        }

        // Add the node to the graph
        add_node_to_graph(G, node);
    }

    file.close();

    return G;
}
