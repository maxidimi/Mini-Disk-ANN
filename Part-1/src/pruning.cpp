#include "../include/header.h"

Graph robust_pruning(Graph &G, Data &p, Dataset &V, double a, int R) {
    /*
        p_ is the point in V that minimizes the distance to p (p*)
        i is the iterator for the loop (p')
        p is the data point in the graph from the input
    */
    // V <- (V U N_out(p)) \ {p} withouth duplicates
    Graph_Node P = find_node_in_graph(G,p); Graph N_out_p = P->out_neighbours;
    for (const auto &i : N_out_p) {
        if (find(V.begin(), V.end(), i->data) == V.end()) {
            V.push_back(i->data);
        }
    }
    auto iter = find(V.begin(), V.end(), p);
    if (iter != V.end()) {
        V.erase(iter);
    }
    
    // N_out(p) <- {}
    N_out_p.clear();
        
    // While V not empty
    while (!V.empty()) {
        // Find p* <- argmin_{p \in V} d(p,q)
        Data p_ = V.front();
        data_t min = euclidean_distance(p,p_);
        for (const auto &i : V) { // i = p'
            data_t dist = euclidean_distance(p,i);
            if (dist < min) {
                min = dist;
                p_ = i;
            }
        }

        // Find node p* in graph and add it to N_out(p) if not already present
        Graph_Node P_ = find_node_in_graph(G,p_);
        if (P_ == nullptr) {
            cout << "Point not found in the graph\n";
            return G;
        }
        if (find(N_out_p.begin(), N_out_p.end(), P_) == N_out_p.end())
            N_out_p.push_back(P_);

        // If |N_out(p)| = R, break
        if ((int)N_out_p.size() == R) break;

        // For p' in V
        Dataset V_dup = V;
        for (const auto &i : V_dup) { // i = p'
            // If a*d(p*, p') <= d(p,p')
            if ((data_t)a*euclidean_distance(p_,i) <= euclidean_distance(p, i)) {
                // Remove p' from V
                auto iter = find(V.begin(), V.end(), i);
                if (iter != V.end()) {
                    V.erase(iter);
                }
            }
        }
        
    }
    
    // Add edges from p to N_out(p)
    P->out_neighbours.clear();
    for (const auto &i : N_out_p) {
        add_edge_to_graph(P,i);
    }

    // Return the graph
    return G;
}
