#include "../include/header.h"

// L\V operation, returns the elements in L that are not in V
void L_m_V(const vector<int> &L, const vector<int> &V, vector<int> &LV) {
    
    unordered_set<int> v_set(V.begin(), V.end());
    LV.clear(); LV.reserve(L.size());

    for (const auto& node : L) {
        // If the node is not in V, add it to the result
        auto it = v_set.find(node);
        if (it == v_set.end()) {
            LV.push_back(node);
        }
    }
}

// Filtered Greedy Algorithm - returning [k-nearest aprx. points, visited points]
pair<vector<int>, vector<int>> filtered_greedy_search(const Graph &G, vector<int> S, Data q, int k, int L_s, const vector<int> &C, vector<int> fq){
    // Initialize sets L<-{}, V<-{}
    vector<int> L; 
    vector<int> V;

    // for s \in S do
    for (const auto &s : S) {
        // If (F_s \cap F_q) != {} then L <- L U {s}
        if (find(fq.begin(), fq.end(), C[s]) != fq.end()) {
            L.push_back(s);
        }
    }
    
    // Initialize L \ V = L, as V is empty
    vector<int> L_not_V; L_not_V.assign(L.begin(), L.end());
    
    // While L \ V is not empty
    while (!L_not_V.empty()) {

        // Find p* <- argmin_{p \in L \ V} d(p,q)
        int p_star = find_min_dist(G, L_not_V, q);

        //V <- V U (p*)
        V.push_back(p_star);

        // Let N'_out(p*) <- {p' \in N_out(p*) : F_p' \cap F_q != {} && p' \notin V}
        unordered_set<int> N_;
        for (const auto &p_tmp : G[p_star]->out_neighbours){
            if ((find(fq.begin(), fq.end(), C[p_tmp]) != fq.end()) && (find(V.begin(), V.end(), p_tmp) == V.end())){
                N_.insert(p_tmp);
            }
        }
        
        // Update L <- L U N'_out(p*)
        for (const auto &p : N_){
            if (find(L.begin(), L.end(), p) == L.end()){
                L.push_back(p);
            }
        }
        
        // If |L| > L_s then update L to retain closest L_s points to q
        if (L.size() > (size_t)L_s && L.size() > 0) {
            // Use partial sort instead of full sort
            auto mid = L.begin() + L_s;
            nth_element(L.begin(), mid, L.end(), [&q, &G](int a, int b) {
                return euclidean_distance(G[a]->data, q) < euclidean_distance(G[b]->data, q);
            });

            L.resize(L_s); // Keep only the top-L_s elements
        }

        // Update L \ V
        L_m_V(L, V, L_not_V);
    }
    
    // Return the first k elements of L
    if (L.size() > (size_t)k) {
        if (L.empty()) return {{}, V}; // No need to proceed if L is empty

        // Use partial sort instead of full sort
        auto mid = L.begin() + k;
        nth_element(L.begin(), mid, L.end(), [&q, &G](int a, int b) {
            return euclidean_distance(G[a]->data, q) < euclidean_distance(G[b]->data, q);
        });

        L.resize(k);
    }
    
    // Return the result as a pair
    return {L, V};
}

// Greedy Algorithm - returning [k-nearest aprx. points, visited points]
pair<vector<int>, vector<int>> greedy_search(const Graph &G, Graph_Node s, Data q, int k, int L_s){

    // Initialize sets L<-{s}, V<-{}
    vector<int> L; L.push_back(s->indx);
    vector<int> V;

    // Initialize L \ V = {s}
    vector<int> L_not_V = {s->indx};
    
    // While L \ V is not empty
    while (!L_not_V.empty()) {

        // Find p* <- argmin_{p \in L \ V} d(p,q)
        int p_star = find_min_dist(G, L_not_V, q);

        // Update L <- L U N_out(p*), V <- V U {p*}
        for (const auto &p : G[p_star]->out_neighbours) {
            if (find_if(L.begin(), L.end(), [p](int i) { return i == p; }) == L.end()) {
                L.push_back(p);
            }
        }
        V.push_back(p_star);

        // If |L| > L_s then update L to retain closest L_s points to q
        if (L.size() > (size_t)L_s) {
            // Use partial sort instead of full sort
            auto mid = L.begin() + L_s;
            nth_element(L.begin(), mid, L.end(), [&q, &G](int a, int b) {
                return euclidean_distance(G[a]->data, q) < euclidean_distance(G[b]->data, q);
            });

            L.resize(L_s);
        }

        // Update L \ V
        L_m_V(L, V, L_not_V);
    }
    
    // Return the first k elements of L
    if (L.size() > (size_t)k) {
        auto mid = L.begin() + k;
        nth_element(L.begin(), mid, L.end(), [&q, &G](int a, int b) {
            return euclidean_distance(G[a]->data, q) < euclidean_distance(G[b]->data, q);
        });

        L.resize(k);
    }
    
    // Return the result as a pair
    return {L, V};
}
