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
    q_idx=
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
    int k = 0, R = -1, L = -1, q_idx = -1; double a = 0.0;
    
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
                else if (key == "q_idx") q_idx = stoi(value);
                else {
                    cerr << "Invalid configuration key: " << key << endl;
                    return 1;
                }
            }
        }
    }
    configFile.close();
    
    // Read the dataset, queries and groundtruth
    Dataset dataset = fvecs_read(dataset_f);
    Dataset queries = fvecs_read(query_f);
    vector<vector<int>> groundtruth_i = ivecs_read(groundtruth_f);
    vector<vector<int>> groundtruth;
    Dataset queries_to_test;
    
    // Set queries and groundtruth to test
    int idx = 0;
    if (q_idx == -1) { // Test all queries in the query file
        queries_to_test = queries;
        groundtruth = groundtruth_i;
        
    } else if (q_idx == -2) { // Test a random query
        idx = rand() % queries.size();
        queries_to_test.push_back(queries[idx]);
        groundtruth.push_back(groundtruth_i[idx]);

    } else { // Test a specific query
        if (q_idx < 0 || q_idx >= static_cast<int>(queries.size())) {
            cerr << "Invalid query index, give [0, " << queries.size() - 1 << "]" << endl;
            return 1;
        } else {
            queries_to_test.push_back(queries[q_idx]);
            groundtruth.push_back(groundtruth_i[q_idx]);
        }
    }
    
    // Check that parametres are valid
    int n = static_cast<int>(dataset.size());
    int d = static_cast<int>(dataset.front().size());
    if (R < 1 || L < k || k <= 0 || a < 1.0 || n <= 0) {
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
    cout << " || Size: " << n << endl;
    cout << " || Dimension: " << d << endl;
    if (q_idx == -1) {
        cout << " || Queries: All" << endl;
    } else {
        cout << " || Queries: " << idx << endl;
    }
    cout << "=======================================================================================\n";
    
    // Start the timer
    clock_t start = clock();
    
    // Create the Vamana index
    Graph G = read_graph("graph.bin");
    if (G.empty()) {
        G = vamana_indexing(dataset, a, L, R);
        store_graph(G, "graph.bin");
    }
    
    time_elapsed(start, "Vamana Indexing");
    cout << "=======================================================================================\n";
    
    for (size_t i = 0; i < queries_to_test.size(); i++) {
        Data query = queries_to_test[i];
        vector<int> groundtruth_t;
        clock_t gr_start = clock();

        // Perform greedy search starting from the first node of the graphß
        Graph_Node s = G.front();
        auto result_p = greedy_search(G, s, query, k, L);
        auto result = result_p.first; auto visited = result_p.second;

        // Print time for each Greedy call
        time_elapsed(gr_start, "Greedy Search " + to_string(i + 1) + "/" + to_string(queries_to_test.size()));
        
        // Print the results
        check_results(dataset, query, result, k, groundtruth_t);
    }
    
    // Print time for both Vamana and Greedy calls
    time_elapsed(start, "both Vamana and Greedy calls");

    // Free the memory allocated for the graph
    for (const auto &node : G) {
        delete node;
    }

    //_CrtDumpMemoryLeaks();

    return 0;
}
