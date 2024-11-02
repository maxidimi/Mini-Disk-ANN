#include "../include/header.h"

// L\V operation, returns the elements in L that are not in V
void L_m_V(const Graph &L, const Graph &V, Graph &LV) {
    
    unordered_set<Graph_Node> v_set(V.begin(), V.end());
    LV.clear(); LV.reserve(L.size());

    for (const auto& node : L) {
        // If the node is not in V, add it to the result
        auto it = v_set.find(node);
        if (it == v_set.end()) {
            LV.push_back(node);
        }
    }
}

// Greedy Algorithm - returning [k-nearest aprx. points, visited points]
pair<Graph,Graph> greedy_search(Graph_Node s, Data q, int k, int L_s){

    // Initialize sets L<-{s}, V<-{}
    Graph L; L.push_back(s);
    Graph V = {};

    // Initialize L \ V = {s}
    Graph L_not_V;
    L_not_V.push_back(s);
    
    // While L \ V is not empty
    while (!L_not_V.empty()) {

        // Find p* <- argmin_{p \in L \ V} d(p,q)
        euclidean_t min_dist = numeric_limits<euclidean_t>::max();
        Graph_Node p_star = nullptr;
        for (const auto &p : L_not_V) {
            euclidean_t dist = euclidean_distance(p->data, q);
            if (dist < min_dist) {
                min_dist = dist;
                p_star = p;
            }
        }

        // Update L <- L U N_out(p*), V <- V U {p*}
        for (const auto &p : p_star->out_neighbours) {
            if (find(L.begin(), L.end(), p) == L.end()) {
                L.push_back(p);
            }
        }
        V.push_back(p_star);

        // If |L| > L_s then update L to retain closest L_s points to q
        if (L.size() > static_cast<size_t>(L_s)) {
            sort(L.begin(), L.end(), [q](const auto &a, const auto &b) {
                return euclidean_distance(a->data, q) < euclidean_distance(b->data, q);
            });
            L.resize(L_s);
        }

        // Update L \ V
        L_m_V(L, V, L_not_V);
    }
    
    // Return the first k elements of L
    if (L.size() > static_cast<size_t>(k)) {
        sort(L.begin(), L.end(), [q](const auto &a, const auto &b) {
            return euclidean_distance(a->data, q) < euclidean_distance(b->data, q);
        });
        L.resize(k);
    }
    
    // Return the result as a pair
    return {L, V};
}