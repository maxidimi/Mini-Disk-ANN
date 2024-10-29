#include "../include/header.h"

Graph robust_pruning(Graph &G, Data &p, Dataset &V, double a, int R) {
    
    // V <- (V U N_out(p)) \ {p} withouth duplicates
    Graph_Node p_node = find_node_in_graph(G, p);

    for (const auto& node : p_node->out_neighbours) {
        if (find(V.begin(), V.end(), node->data) == V.end()) {
            V.push_back(node->data);
        }
    }

    V.erase(remove(V.begin(), V.end(), p), V.end());
    
    // N_out(p) <- {}
    p_node->out_neighbours.clear();
    p_node->out_neighbours.reserve(R);
    
    // While V not empty
    while (!V.empty()) {

        // Find p* <- argmin_{p \in V} d(p,q)
        Data p_star = {};
        long double min_distance = numeric_limits<long double>::max();
        
        for (const auto& p_tmp : V) {
            long double distance = euclidean_distance(p, p_tmp);
            if (distance < min_distance) {
                min_distance = distance;
                p_star = p_tmp;
            }
        }

        // Find node p* in graph and add it to N_out(p) if not already present
        Graph_Node p_star_node = find_node_in_graph(G, p_star);

        p_node->out_neighbours.insert(p_star_node);
        
        // If |N_out(p)| = R, break
        if (static_cast<int>(p_node->out_neighbours.size()) == R) break;
        
        // For p' in V
        Dataset V_tmp = V;
        for (const auto& p_tmp : V_tmp) {
            // If a*d(p*,p') <= d(p,p') then remove p' from V
            if (static_cast<long double>(a * euclidean_distance(p_star, p_tmp)) <= euclidean_distance(p, p_tmp)) {
                V.erase(find(V.begin(), V.end(), p_tmp));
            }
        }
    }

    return G;
}
