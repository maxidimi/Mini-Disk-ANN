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
    int k = 0, R = -1, L = -1, q_idx = -3;
    double a = 0.0; string vam_func = "error";
    string graph_name = "graph.bin";
    bool toPrint = false;
    
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
                else if (key == "toPrint") toPrint = (value == "true");
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
    Dataset dataset, queries; vector<vector<int>> groundtruth;
    vector<int> C, V, F;
    if (vam_func == "vamana") {
        dataset = fvecs_read(dataset_f);

        if (q_idx != -3) {
            queries = fvecs_read(query_f);
            groundtruth = ivecs_read(groundtruth_f);
        }
    } else {
        auto r = read_sigmod_dataset(dataset_f); 
        dataset = r.first; C = r.second;

        if (q_idx != -3) {
            auto r2 = read_sigmod_queries(query_f);
            queries = r2.first; V = r2.second;

            groundtruth = read_sigmod_groundtruth(groundtruth_f);
            if (groundtruth.empty()) groundtruth = find_store_groundtruth(r, r2, groundtruth_f);
        }
        
        set<int> C_set(C.begin(), C.end());
        for (int c : C_set) F.push_back(c);
    }

    // Exit if user wants only to compute the groundtruth
    if (q_idx == -4) return 0;

    // Check that parametres are valid
    int n = (int)dataset.size();
    int d = (int)dataset.front().size();
    if (R < 1 || L < k || k <= 0 || a < 1.0 || n <= 0 || d <= 0 || q_idx < -4) {
        cerr << "Invalid parametrization!" << endl;
        return 1;
    }
    
    // Set queries and groundtruth to test
    vector<int> indx_to_test;
    if (q_idx != -3) {
        if (q_idx == -1) { // Test all queries in the query file
            for (int i = 0; i < (int)queries.size(); i++) indx_to_test.push_back(i);

        } else if (q_idx == -2) { // Test a random query
            q_idx = rand() % queries.size();
            indx_to_test.push_back(q_idx);

        } else { // Test a specific query
            if (q_idx < 0 || q_idx >= (int)queries.size()) {
                cerr << "Invalid query index, give [0, " << queries.size() - 1 << "]" << endl;
                return 1;
            } else indx_to_test.push_back(q_idx);
        }
    }
    
    if (toPrint) {
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
            cout << " || Query Index: " << q_idx << endl;
            cout << " || Label: " << (vam_func == "vamana" ? "Unfiltered" : (V[q_idx] == -1 ? "Unfiltered" : "Filtered")) << endl;
        }
        cout << "=======================================================================================\n";
    }
    
    // Start the timer
    auto start = high_resolution_clock::now();

    // Create/Read the Vamana index based on the prefered function and store it
    Graph G;
    G = read_graph(graph_name + ".bin");
    if (G.empty()) {
        if (vam_func == "vamana") G = vamana_indexing(dataset, a, L, R);
        else if (vam_func == "filtered") G = filtered_vamana_indexing(dataset, C, a, L, R, F);
        else G = stitched_vamana_indexing(dataset, C, a, L, R, F);
        store_graph(G, graph_name + ".bin");
    }

    if (toPrint) time_elapsed(start, "Vamana Indexing");
    auto indexing_time = high_resolution_clock::now() - start;
    auto indexing_secs = duration_cast<seconds>(indexing_time).count();

    // Find size of graph stored
    ifstream graphFile(graph_name + ".bin", ios::binary | ios::ate);
    size_t size = graphFile.tellg();
    graphFile.close();
    double sizeMB = (double)size / 1024 / 1024;

    if (q_idx != -3) { // If user wants search
        if (toPrint) {
            cout << "=======================================================================================\n";
            cout << "                                      Results:" << endl;
            cout << "=======================================================================================\n";
        }
        
        // Find the medoid for each label
        int queries_size = (int)indx_to_test.size();
        unordered_map<int, int> medoid_map;
        int filtered_count = 0; int unfiltered_count = queries_size;
        if (vam_func != "vamana") {
            medoid_map.reserve(F.size());
            medoid_map = find_medoid(dataset, C, 1, F);
            
            unfiltered_count = count(V.begin(), V.end(), -1);
            filtered_count = (int)V.size() - unfiltered_count;
        }
        
        // Perform the Greedy search for each query
        double recall_sum = 0.0;
        double filtered_recall_sum = 0.0; 
        double unfiltered_recall_sum = 0.0;
        double recall_sum_time = 0.0;

        auto queries_start = high_resolution_clock::now();

        #pragma omp parallel for schedule(static) if (queries_size > 100) reduction(+:recall_sum, filtered_recall_sum, unfiltered_recall_sum, recall_sum_time)
            for (const auto &i : indx_to_test) {
                auto time_start = high_resolution_clock::now();
                Data query = queries[i];

                vector<int> groundtruth_t = groundtruth[i];

                int V_i = -1;
                if (vam_func != "vamana") V_i = V[i];

                // Perform greedy search starting based on the prefered function
                pair<vector<int>, vector<int>> result_p;

                if (vam_func == "vamana") {
                    Graph_Node s = G.front();

                    result_p = greedy_search(G, s, query, k, L);

                } else {
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

                }

                auto result = result_p.first; auto visited = result_p.second;
                
                // Print the results
                double result_recall = check_results(result, k, groundtruth_t, toPrint);
                
                recall_sum += result_recall;
                if (vam_func != "vamana") {
                    if (V_i == -1) unfiltered_recall_sum += result_recall;
                    else filtered_recall_sum += result_recall;
                } else {
                    unfiltered_recall_sum += result_recall;
                }

                auto time_end = high_resolution_clock::now();
                auto elapsed_secs = duration_cast<milliseconds>(time_end - time_start).count();
                recall_sum_time += elapsed_secs;
            }
        
        // Print the average recall
        if (toPrint) {
            cout << "=======================================================================================\n";
            cout << "===> Average Recall@" << k << ": " << (double)(queries_size == 0 ? 0 : recall_sum/queries_size) << "%" << endl;
            if (vam_func != "vamana" && queries_size > 1) {
                cout << "===> Average Recall@" << k << " for unfiltered: " << (double)(unfiltered_count == 0 ? 0 : unfiltered_recall_sum/unfiltered_count) << "%" << endl;
                cout << "===> Average Recall@" << k << " for filtered: " << (double)(filtered_count == 0 ? 0 : filtered_recall_sum/filtered_count) << "%" << endl;
            }
            cout << "=======================================================================================\n";
        }

        // Print the average search time
        auto elapsed_secs = high_resolution_clock::now() - queries_start;
        if (!toPrint) {
            auto secs = duration_cast<seconds>(elapsed_secs).count();
            cout << (double)(queries_size == 0 ? 0 : recall_sum/queries_size) << ","; // Recall@k
            cout << (double)(unfiltered_count == 0 ? 0 : unfiltered_recall_sum/unfiltered_count) << ","; // Recall@k for unfiltered
            cout << (double)(filtered_count == 0 ? 0 : filtered_recall_sum/filtered_count) << ","; // Recall@k for filtered
            cout << (double)(secs == 0 ? queries_size : queries_size/secs) << ","; // Queries per second
            cout << (double)(queries_size == 0 ? 0 : recall_sum_time/queries_size) << ","; // Average search time
            cout << (double)(indexing_secs) << ","; // Indexing time
            cout << std::fixed << std::setprecision(2) << sizeMB << endl; // Graph size
        }
    } else {
        // Print the average indexing time
        if (!toPrint) cout << "0,0,0,0,0," << (double)(indexing_secs) << "," << std::fixed << std::setprecision(2) << sizeMB << endl;
    }

    // Free the memory allocated for the graph
    for (const auto &node : G) delete node;

    return 0;
}
