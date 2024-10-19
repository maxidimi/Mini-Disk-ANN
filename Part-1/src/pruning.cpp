#include "../include/header.h"

Graph robust_pruning(Graph G, Data p, Dataset V, int a, int R) {
    Graph_Node P = find_node_in_graph(G,p);
    if(P == NULL) return G;
    list<struct graph_node *> N_out_p;
    N_out_p = P->out_neighbours;
    
    
    auto iter = find(V.begin(), V.end(), p);
    if(iter != V.end()){
        V.erase(iter);
    }

    for (auto i : N_out_p) {
        if (i->data != p) {
            V.push_back(i->data);        
        }
    }
    
    N_out_p.clear();
    P->out_neighbours.clear();
    
        

    while ( V.empty() == 0) {
        int min = euclidean_distance(p,V.front());
        Data p_ = V.front();

        for(auto i : V) {
            int dist = euclidean_distance(p,i);
            if(dist < min) {
                min = dist;
                p_ = i;
            }
        }
        Graph_Node P_ = find_node_in_graph(G,p_);
        N_out_p.push_back(P_);

        if(static_cast<int>(N_out_p.size()) == R) break;

        for(auto i : V) {
            
            double d1 = euclidean_distance(i,p),d2 = euclidean_distance(i,p_);
            if((a*d2) <= d1) {
                auto iter = find(V.begin(), V.end(), p);
                if(iter != V.end()){
                    V.erase(iter);
                }
            }
        }
        
    }
    for(auto i : N_out_p) {
        add_edge_to_graph(P,i);
    }

    return G;
}

// Robust Prune