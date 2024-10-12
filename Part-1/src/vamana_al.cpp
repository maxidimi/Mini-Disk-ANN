#include "../include/header.h"

// Vamana Indexing Algorithm
// Gets database P, a, L, R
// Returns directed graph G over P with out-degree <= R

// Medoid of a dataset P is the point s in P that minimizes the sum of distances to all other points in P
Data medoid(Dataset P) {
    
    Data s = P.front();

    int min_distance = 0;

    for (auto p : P) {
        // distance from p to all other points in P
        int distance = 0;
        for (auto q : P) {
            distance += euclidean_distance(p, q);
        }

        if (distance < min_distance) {
            s = p;
            min_distance = distance;
        }
    }

    return s;
}

Graph vamana_indexing(Dataset P, int a, int L, int R) {
    Graph graph;

    // Initialize G to a random R-regular directed graph
    for (auto p : P) {
        Graph_Node node = create_graph_node(p);
        add_node_to_graph(graph, node);
    }

    // Let s denote the medoid of dataset P
    Graph_Node s = find_node_in_graph(graph, medoid(P));

    // Let σ denote a random permutation of 1...n
    vector<int> sigma;
    random_permutation(sigma, P);

    // For each point p in σ do
    int i = 0;
    for (auto s_i : sigma) {
        // Let s_i denote the i-th point in σ and s_iter 
        auto s_item = P.begin();
        advance(s_item, s_i);

        Graph_Node s_iter = find_node_in_graph(graph, *s_item);
        graph = robust_pruning(graph, s_iter->data, P, a, R);

        // For all points j in N_out(σ(i)) do
        for (auto s_j : sigma) {
            // If |N_out(σ(i)) U {σ(i)}| > R then
                // Run robust_prune(j, N_out(j) U {σ(i)}, a, R)
            // Else
                // N_out(j) = N_out(j) U {σ(i)}
        }
        
    }
    
    // Return the graph
    return graph;
}