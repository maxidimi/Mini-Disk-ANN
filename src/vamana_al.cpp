#include "../include/header.h"

// Vamana Indexing Algorithm
// Gets database P, a, L, R
// Returns directed graph G over P with out-degree <= R

// Medoid of a dataset P is the point s in P that minimizes the sum of distances to all other points in P
int medoid(const Dataset &P) {

    /* Compute the medoid of the dataset P as:
        s = argmin_{p \in P} \sum_{q \in P, q != p} d(p, q)
        Creates a vector sum of distances from each point to all other points.
        Computes only the upper triangular matrix of distances.
        Returns the index of the point with the minimum sum.
    */
    
    size_t n = P.size(); int medoid_index = -1;

    euclidean_t min_sum = numeric_limits<euclidean_t>::max();

    // Initialize the sum vector
    vector<euclidean_t> sum(n, 0.0L);

    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            if (i != j) {
                // Store the distance between points i and j in each sum
                euclidean_t dist = euclidean_distance(P[i], P[j]);
                sum[i] += dist;
                sum[j] += dist;
            }
        }
        
        if (sum[i] < min_sum) {
            min_sum = sum[i];
            medoid_index = i;
        }
    }
    
    // Return the index of the medoid
    return medoid_index;
}

// Vamana Indexing Algorithm
Graph vamana_indexing(const Dataset &P, double a, int L, int R) {
    size_t n = P.size();

    // Initialize G to a random R-regular directed graph
    Graph G; G.reserve(n);

    for (auto &p : P) {
        Graph_Node node = create_graph_node(p, R);
        add_node_to_graph(G, node);
    }
    
    // Select up to R unique random neighbours
    int i = 0;
    for (auto &node : G) {
        
        set<int> random_indices;

        while (static_cast<int>(random_indices.size()) < R) {
            int random_idx = rand() % n;

            if (random_idx == i) continue;

            random_indices.insert(random_idx);
        }

        for (const auto &idx : random_indices) {
            add_edge_to_graph(node, idx);
        }i++;
    }
    
    // Let s denote the medoid of dataset P
    int s_index = medoid(P);
    
    Data s_d = P[s_index];
    
    Graph_Node s = G[s_index];
    
    // Let σ denote a random permutation of P
    vector<int> sigma = random_permutation(n);
    
    // For all points i in σ do
    for (const auto &i : sigma) {

        // Node of graph with data p
        Graph_Node p = G[i];
        Data p_d = P[i];

        // Run greedy_search, V are the visited nodes, L_res are the nearest neighbours
        pair<Graph, Graph> result = greedy_search(G, s, p_d, 1, L);

        Graph L = result.first; Graph V = result.second;

        // Run robust_prune to update out-neighbours of s_i
        Dataset V_d = get_data(V);

        G = robust_pruning(G, p_d, V_d, a, R, p);

        // For all points j in N_out(σ(i)) do
        for (auto j : p->out_neighbours) {
            // |N_out(j) U {σ(i)}|
            auto N_out_j_p = G[j]->out_neighbours;
            N_out_j_p.insert(i);

            // If |N_out(σ(i)) U {σ(i)}| > R then
            if (static_cast<int>(N_out_j_p.size()) > R) {
                // Run robust_prune to update out-neighbours of j
                Dataset N_out_j_p_d;
                for (const auto &node : N_out_j_p) {
                    N_out_j_p_d.push_back(G[node]->data);
                }

                G = robust_pruning(G, G[j]->data, N_out_j_p_d, a, R, G[j]);
            } else {
                // Add σ(i) to out-neighbours of j
                G[j]->out_neighbours.insert(i);
            }
        }
    }
    
    // Return the graph
    return G;
}
