#include "../include/header.h"

// Vamana Indexing Algorithm
// Gets database P, a, L, R
// Returns directed graph G over P with out-degree <= R

// Medoid of a dataset P is the point s in P that minimizes the sum of distances to all other points in P
Data medoid(Dataset &P) {
    
    Data s = P.front();

    data_t min_distance = numeric_limits<data_t>::max();

    for (const auto &p : P) {
        // Compute distance from p to all other points in P
        data_t distance = 0;
        for (const auto &q : P) {
            if (p == q) continue;
            distance += euclidean_distance(p, q);
        }

        if (distance < min_distance) {
            s = p;
            min_distance = distance;
        }
    }
    
    return s;
}

// Vamana Indexing Algorithm
Graph vamana_indexing(Dataset &P, double a, int L, int R) {
    /*s is the medoid of dataset P
      i is the index given by the random permutation σ
      p is the i-st point in dataset P
      p_in_graph is the node of graph with data p
    */
    Graph graph;
    int n = (int)P.size();

    // Initialize G to a random R-regular directed graph
    for (auto &p : P) {
        Graph_Node node = create_graph_node(p);
        add_node_to_graph(graph, node);
    }
    srand((unsigned int)time(0));
    
    // Select up to R unique random neighbours
    for (const auto &node : graph) {
        //int num_edges = rand() % R + 1;
        set<int> random_indices;
        while ((int)random_indices.size() < R) {
            int random_idx = rand() % n;
            if (random_indices.find(random_idx) == random_indices.end()) {
                random_indices.insert(random_idx);
                Graph_Node rand_node = get_node_at_index(graph, P, random_idx);
                add_edge_to_graph(node, rand_node);
            }
        }
    }
    
    // Let s denote the medoid of dataset P
    Data s_d = medoid(P);
    
    Graph_Node s = find_node_in_graph(graph, s_d);
    
    // Let σ denote a random permutation of P
    vector<int> P_perm = random_permutation(n);
    
    // For all points i in σ do
    for (const auto &i : P_perm) {

        // Node of graph with data p
        Data p = P[i];
        
        Graph_Node p_in_graph = find_node_in_graph(graph, p);

        // Run greedy_search, V are the visited nodes, L_res are the nearest neighbours
        pair<Graph, Graph> res = greedy_search(s, p, 1, L);
        Graph L_res = res.first; Graph V = res.second;

        // Run robust_prune to update out-neighbours of s_i
        Dataset V_data = get_data(V);
        graph = robust_pruning(graph, p, V_data, a, R);

        // For all points j in N_out(σ(i)) do
        Graph p_d = p_in_graph->out_neighbours;
        for (const auto &j : p_d) {
            // |N_out(j) U {σ(i)}|
            Graph N_out_j = j->out_neighbours;
            N_out_j.push_back(p_in_graph);

            // If |N_out(σ(i)) U {σ(i)}| > R then
            if (N_out_j.size() > (size_t)R) {
                // Run robust_prune to update out-neighbours of j
                Dataset N_out_j_data = get_data(N_out_j);
                graph = robust_pruning(graph, j->data, N_out_j_data, a, R);
            } else {
                // Add σ(i) to out-neighbours of j
                add_edge_to_graph(j, p_in_graph);
            }
        }
    }
    
    // Return the graph
    return graph;
}
