#include "../include/header.h"

Graph robust_pruning(Graph &G, Graph_Node &p_node, vector<int> &V, double a, int R) {
    Data p = p_node->data;
    
    // Add N_out(p) to V without duplicates
    for (const auto& p_tmp : p_node->out_neighbours) {
        if (find(V.begin(), V.end(), p_tmp) == V.end()) {
            V.push_back(p_tmp);
        }
    }

    // Remove p from V
    V.erase(remove(V.begin(), V.end(), p_node->indx), V.end());
    
    // Clear N_out(p) and reserve space for R neighbors
    p_node->out_neighbours.clear();

    // While V is not empty
    while (!V.empty()) {
        
        // Find p* <- argmin_{p ∈ V} d(p, q)
        int p_star = find_min_dist(G, V, p);

        // Find node p* in graph and add it to N_out(p) if not already present
        Graph_Node p_star_node = G[p_star];

        if (p_star_node) {
            p_node->out_neighbours.insert(p_star_node->indx);
        }

        // If N_out(p) reaches size R, break
        if (p_node->out_neighbours.size() == (size_t)R) break;

        // For each p' in V and delete p' if a * d(p*, p') <= d(p, p')
        V.erase(remove_if(V.begin(), V.end(), [&G, &p, &a, &p_star](int p_tmp) {
            return a * euclidean_distance(G[p_star]->data, G[p_tmp]->data) <= euclidean_distance(p, G[p_tmp]->data);
        }), V.end());
    }

    return G;
}
