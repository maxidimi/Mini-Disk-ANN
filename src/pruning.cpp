#include "../include/header.h"

Graph robust_pruning(Graph &G, Graph_Node &p_node, Dataset &V, double a, int R) {
    Data p = p_node->data;
    
    // Add N_out(p) to V without duplicates
    for (const auto& neighbor_index : p_node->out_neighbours) {
        Data neighbor_data = G[neighbor_index]->data;
        if (find(V.begin(), V.end(), neighbor_data) == V.end()) {
            V.push_back(neighbor_data);
        }
    }

    // Remove p from V
    V.erase(remove(V.begin(), V.end(), p), V.end());
    
    // Clear N_out(p) and reserve space for R neighbors
    p_node->out_neighbours.clear();

    // While V is not empty
    while (!V.empty()) {
        
        // Find p* <- argmin_{p ∈ V} d(p, q)
        Data p_star = {};
        euclidean_t min_distance = numeric_limits<euclidean_t>::max();

        for (const auto& p_tmp : V) {
            euclidean_t distance = euclidean_distance(p, p_tmp);
            if (distance < min_distance) {
                min_distance = distance;
                p_star = p_tmp;
            }
        }

        // Find node p* in graph and add it to N_out(p) if not already present
        Graph_Node p_star_node = find_data_in_graph(G, p_star);

        if (p_star_node) {
            p_node->out_neighbours.insert(p_star_node->indx);
        }

        // If N_out(p) reaches size R, break
        if (p_node->out_neighbours.size() == (size_t)R) break;

        // For each p' in V, check distance condition and remove if met
        Dataset V_tmp = V;
        for (const auto& p_tmp : V_tmp) {
            if (a * euclidean_distance(p_star, p_tmp) <= euclidean_distance(p, p_tmp)) {
                auto it = find(V.begin(), V.end(), p_tmp);
                if (it != V.end()) {
                    V.erase(it);
                }
            }
        }
    }

    return G;
}
