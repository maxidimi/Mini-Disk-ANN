#include "../include/header.h"

// Greedy Algorithm - returning [k-nearest aprx. points, visited points]
pair<list<Graph_Node>,list<Graph_Node>> greedy_search(Graph_Node s, Data q, int k, int L){

    list<Graph_Node> visited_list, searching_list, lv;

    // Initialize sets L<-{s}, V<-{}
    searching_list.push_back(s); 

    lv = L_m_V(searching_list, visited_list);
    
    // While L \ V is not empty
    while (!lv.empty()) {
        // Let node p* <- argmin_{p \in L\V} d(p.q)
        double mindist;
        mindist = euclidean_distance(q, lv.front()->data);

        // Find the node with minimum distance
        Graph_Node p = lv.front();
        for (auto i : lv) {
            double dist = euclidean_distance(i->data, q);
            if (dist < mindist) {
                mindist = dist;
                p = i; 
            }    
        }

        // Update L <- L U N_out(p*) by adding lists instead of looping and prevent duplicates
        set<Graph_Node> temp(searching_list.begin(), searching_list.end());
        temp.insert(p->out_neighbours.begin(), p->out_neighbours.end());
        searching_list.clear();
        searching_list.assign(temp.begin(), temp.end());

        // Update V <- V U {p*}
        visited_list.push_back(p);
        
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
    searching_list.resize(k);

    // Return the result as a pair
    pair<list<Graph_Node>,list<Graph_Node>> result;
    result.first = searching_list; result.second = visited_list;

    return result;
}
