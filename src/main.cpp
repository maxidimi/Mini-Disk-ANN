#include "../include/header.h"

// More info about configuration file can be found in the README.md.

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
    int k = 0, R = -1, L = -1, q_idx = -1;
    double a = 0.0; string vam_func = "error";
    string graph_name = "graph.bin";
    bool print = false;
    
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
                else if (key == "graph_name") graph_name = value;
                else if (key == "vamana_function") vam_func = value;
                else {
                    cerr << "Invalid configuration key: " << key << endl;
                    return 1;
                }
            }
        }
    }
    configFile.close();

    if (vam_func != "vamana" && vam_func != "filtered" && vam_func != "stitched") {
        cerr << "Invalid Vamana function!" << endl;
        return 1;
    }

    // Read the dataset, queries and groundtruth etc.
    Dataset dataset, queries; vector<vector<int>> groundtruth_i;
    vector<int> C, V, F;
    if (vam_func == "vamana") {
        dataset = fvecs_read(dataset_f);

        queries = fvecs_read(query_f);

        groundtruth_i = ivecs_read(groundtruth_f);
    } else {
        auto r = read_sigmod_dataset(dataset_f); 
        dataset = r.first; C = r.second;

        auto r2 = read_sigmod_queries(query_f);
        queries = r2.first; V = r2.second;

        groundtruth_i = read_sigmod_groundtruth(groundtruth_f);
        if (groundtruth_i.empty()) groundtruth_i = find_store_groundtruth(r, r2, groundtruth_f);

        set<int> C_set(C.begin(), C.end());
        for (int c : C_set) F.push_back(c);
    }

    // Check that parametres are valid
    int n = (int)dataset.size();
    int d = (int)dataset.front().size();
    if (R < 1 || L < k || k <= 0 || a < 1.0 || n <= 0 || d <= 0) {
        cerr << "Invalid parametrization!" << endl;
        return 1;
    }
    
    // Set queries and groundtruth to test
    vector<vector<int>> groundtruth;
    vector<int> V_to_test; Dataset queries_to_test;
    
    if (q_idx == -1) { // Test all queries in the query file
        queries_to_test = queries;
        groundtruth = groundtruth_i;
        if (vam_func != "vamana") V_to_test = V;
        
    } else if (q_idx == -2) { //? Test a random query
        q_idx = rand() % queries.size();
        queries_to_test.push_back(queries[q_idx]);
        groundtruth.push_back(groundtruth_i[q_idx]);
        if (vam_func != "vamana") V_to_test.push_back(V[q_idx]);

    } else { //? Test a specific query
        if (q_idx < 0 || q_idx >= (int)queries.size()) {
            cerr << "Invalid query index, give [0, " << queries.size() - 1 << "]" << endl;
            return 1;
        } else {
            queries_to_test.push_back(queries[q_idx]);
            groundtruth.push_back(groundtruth_i[q_idx]);
            if (vam_func != "vamana") V_to_test.push_back(V[q_idx]);
        }
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
    cout << " || Indexing Function: " << vam_func << endl;
    if (q_idx == -1) {
        cout << " || Queries: All" << endl;
    } else {
        print = true;
        cout << " || Query Index: " << q_idx << endl;
    }
    cout << "=======================================================================================\n";
    
    // Start the timer
    clock_t start = clock();
    
    // Create/Read the Vamana index based on the prefered function and store it
    Graph G; unordered_map<int, Graph> G_stitched;
    if (vam_func == "vamana") {
        G = read_graph(graph_name + ".bin");
        if (G.empty()) {
            G = vamana_indexing(dataset, a, L, R);
            store_graph(G, graph_name + ".bin");
        }

    } else if (vam_func == "filtered") {
        G = read_graph(graph_name + ".bin");
        if (G.empty()) {
            G = filtered_vamana_indexing(dataset, C, a, L, R, F);
            store_graph(G, graph_name + ".bin");
        }

    } else if (vam_func == "stitched") {
        for (const auto &f : F) {
            Graph G_f = read_graph(graph_name + to_string(f) + ".bin");
            if (G_f.empty()) {
                G_stitched = stitched_vamana_indexing(dataset, C, a, L, R, F);
                for (const auto &g : G_stitched) {
                    store_graph(g.second, graph_name + to_string(g.first) + ".bin");
                }
                break;
            }
        }
    }
    time_elapsed(start, "Vamana Indexing");
    cout << "=======================================================================================\n";
    cout << "                                      Results:" << endl;
    cout << "=======================================================================================\n";
    
    // Find the medoid for each label
    unordered_map<int, int> medoid_map; medoid_map.reserve(F.size());
    if (vam_func == "filtered") medoid_map = find_medoid(dataset, C, 1, F);
    
    // Perform the Greedy search for each query
    double recall_sum = 0.0; Graph G_q; int V_i;
    for (size_t i = 0; i < queries_to_test.size(); i++) {
        Data query = queries_to_test[i];

        vector<int> groundtruth_t = groundtruth[i];
        
        cout << " || Query: " << (q_idx != -1 ? q_idx : i);
        if (vam_func != "vamana") {
            V_i = V_to_test[i];
            cout << " | Label: " << V_i << endl;
        } else cout << endl;

        clock_t gr_start = clock();

        // Perform greedy search starting based on the prefered function
        pair<vector<int>, vector<int>> result_p;

        if (vam_func == "vamana") {
            Graph_Node s = G.front();

            result_p = greedy_search(G, s, query, k, L);

        } else if (vam_func == "filtered") {
            vector<int> fq, S;

            if (V_i == -1) { // Search for classic ANN
                // Set medoid for each label the result of greedy search with k = 1 for the subgraph of the every label
                for (const auto &f : F) {
                    auto result_l = filtered_greedy_search(G, {medoid_map[f]}, query, 1, L, C, {f});
                    S.push_back(result_l.first.front());
                }
                fq = F;

            } else {
                S.push_back(medoid_map[V_i]);
                fq = {V_i};
            }

            result_p = filtered_greedy_search(G, S, query, k, L, C, fq);

        } else if (vam_func == "stitched") {
            if (V_i == -1) { // Search for classic ANN
                continue;//?
            } else {
                G_q = G_stitched[V_i];

                Graph_Node s = G_q.front();

                result_p = greedy_search(G_q, s, query, k, L);
            }
        }

        auto result = result_p.first; auto visited = result_p.second;

        // Print time for each Greedy call
        if (print) time_elapsed(gr_start, "Greedy Search " + to_string(i + 1) + "/" + to_string(queries_to_test.size()));
        
        // Print the results
        recall_sum += check_results(dataset, query, result, k, groundtruth_t, true, vam_func, G_q);
    }
    
    // Print time for both Vamana and Greedy calls
    time_elapsed(start, "both Vamana and Greedy calls");

    // Print the average recall
    cout << "=======================================================================================\n";
    cout << "===> Average Recall@" << k << ": " << (double)(recall_sum/queries_to_test.size()) << "%" << endl;
    cout << "=======================================================================================\n";

    // Free the memory allocated for the graph
    for (const auto &node : G) delete node;

    return 0;
}
