#include "../include/header.h"

// Greedy Algorithm - returning [k-nearest aprx. points, visited points]
pair<Graph,Graph> greedy_search(Graph_Node s, Data q, int k, int L){

    // Initialize sets L<-{s}, V<-{}
    Graph visited_list, searching_list, lv;
    searching_list.push_back(s); 

    // Initialize L \ V = {s}
    lv.push_back(s);
    
    // While L \ V is not empty
    while (!lv.empty()) {
        // Let node p* <- argmin_{p \in L\V} d(p.q)
        // Find the node with minimum distance
        Graph_Node p_s = lv.front();
        double mindist = euclidean_distance(p_s->data, q);
        for (const auto &i : lv) {
            double dist = euclidean_distance(i->data, q);
            if (dist < mindist) {
                mindist = dist;
                p_s = i; 
            }    
        }

        // Update L <- L U N_out(p*) and prevent duplicates with set
        if (p_s == nullptr) break;
        set<Graph_Node> temp(searching_list.begin(), searching_list.end());
        for (const auto &neighbour : p_s->out_neighbours) {
            if (find(visited_list.begin(), visited_list.end(), neighbour) == visited_list.end()) {
                temp.insert(neighbour);  // Only add unvisited neighbours
            }
        }
        searching_list.clear();
        searching_list.assign(temp.begin(), temp.end());

        // Update V <- V U {p*}
        visited_list.push_back(p_s);
        
        // If |L| > L, then update lv to be the L closest points to q
        //! Sort the list and keep the first L elements (or pop the last L elements)
        if (searching_list.size() > (size_t)L) {
            searching_list.sort([q](Graph_Node a, Graph_Node b) {
                return euclidean_distance(a->data, q) < euclidean_distance(b->data, q);
            });
            
            searching_list.resize(L);
        }

        lv = L_m_V(searching_list, visited_list);
    }
    
    // Return the first k elements of L
    if (k < (int)searching_list.size()) 
        searching_list.resize(k);

    // Return the result as a pair
    return {searching_list, visited_list};
}
