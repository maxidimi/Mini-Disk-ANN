#include "../include/header.h"

/*
    Make a configuration file with the following format:
    dataset=dataset.fvecs
    query=query.fvecs
    groundtruth=groundtruth.ivecs
    k=10
    R=10
    L=20
    a=1.5
    n=10000
    d=128
    n and d are used for generating random datasets and queries.
    Then run the program with the configuration file as an argument.
*/

int main(int argc, char *argv[]) {
    // Read the configuration file name from the command line
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <config_file>" << endl;
        return 1;
    } char *file_name = argv[1];
    
    ifstream configFile(file_name);
    if (!configFile) {
        cerr << "Unable to open configuration file!" << endl;
        return 1;
    }

    srand((unsigned int)time(0));

    // Read the configuration file and set the parameters
    string dataset_f, query_f, groundtruth_f;
    int k = 0, R = -1, L = -1, n = -1, d = -1; double a = 0.0;
    
    string line;
    while (getline(configFile, line)) {
        istringstream lineStream(line);
        string key;
        if (getline(lineStream, key, '=')) {
            string value;
            if (getline(lineStream, value)) {
                if (key == "dataset") dataset_f = value;
                else if (key == "query") query_f = value;
                else if (key == "groundtruth") groundtruth_f = value;
                else if (key == "k") k = stoi(value);
                else if (key == "R") R = stoi(value);
                else if (key == "L") L = stoi(value);
                else if (key == "a") a = stod(value);
                else if (key == "n") n = stoi(value);
                else if (key == "d") d = stoi(value);
            }
        }
    }
    configFile.close();
    
    // Read the dataset
    Dataset dataset;
    vector<int> groundtruth;
    Data query;
    int ind = rand() % 100;
    if (n != -1 && d != -1) {
        dataset = random_dataset(n, d);
        query = random_query(d);
        groundtruth = {};
    } else {
        dataset = fvecs_read(dataset_f);
        query = fvecs_read(query_f).at(ind);
        Dataset groundtruth_d = ivecs_read(groundtruth_f);
        //convert groundtruth.at(ind) to vector<int>
        for (const auto &i : groundtruth_d.at(ind)) {
            groundtruth.push_back(i);
        }
    }

    n = static_cast<int>(dataset.size());
    if (R == -1  || L == -1 || k  < 0 || a < 1.0) {
        cerr << "Invalid parametrization!" << endl;
        return 1;
    }
    
    cout << " || Dataset: " << dataset_f << endl;
    cout << " || Query: " << query_f << endl;
    cout << " || Groundtruth: " << groundtruth_f << endl;
    cout << " || k: " << k << endl;
    cout << " || R: " << R << endl;
    cout << " || L: " << L << endl;
    cout << " || a: " << a << endl;
    cout << " || Size: " << dataset.size() << endl;
    cout << " || Dimension: " << dataset.front().size() << endl;
    cout << "====================================================================\n";
    
    // Start the timer
    clock_t start = clock();
    
    // Create the Vamana index
    Graph G = vamana_indexing(dataset, a, L, R);
    
    // Perform greedy search starting from the first node
    Graph_Node s = G.front();
    auto result_p = greedy_search(s, query, k, L);
    auto result = result_p.first; auto visited = result_p.second;

    // End the timer
    clock_t end = clock();
    double elapsed_time = double(end - start) / CLOCKS_PER_SEC;
    cout << " || Time taken: " << elapsed_time << " seconds.\n";
    
    check_results_manually(dataset, query, result, k, groundtruth);
    cout << "====================================================================\n";
    check_results_manually(dataset, query, result, k, {});
    
    // Free the memory allocated for the graph
    for (const auto &node : G) {
        delete node;
    }

    return 0;
}
