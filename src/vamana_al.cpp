#include "../include/header.h"

// Find Medoid
unordered_map<int, int> find_medoid(const Dataset &P, vector<vector<int>> F, int threshold) {
    // F is a matrix of size n x FILTERS, where F[i][f] = 1 if point i matches filter f
    size_t n = P.size();

    // Initialize M be an empty map
    unordered_map<int, int> M; M.reserve(FILTERS);

    // Initialize T to a zero map, T is intended as a counter
    unordered_map<int, int> T; T.reserve(n);
    for (size_t i = 0; i < n; i++) T[i] = 0;

    // Foreach f \in F do
    for (int f = 0; f < FILTERS; f++) {

        // Let P_f denote the ids of all points matching filter f
        unordered_set<int> P_f;
        for (size_t i = 0; i < n; i++) {
            if (F[i][f] == 1) P_f.insert(i);
        }

        // Let R_F <- τ randomly sampled data point ids from P_f
        vector<int> R_f(P_f.begin(), P_f.end());
        random_shuffle(R_f.begin(), R_f.end());
        R_f.resize(threshold);

        // p* <- argmin_{p \in R_F} T[p]
        int p_star = -1;
        euclidean_t min_dist = numeric_limits<euclidean_t>::max();
        for (const auto &p : R_f) {
            if (T[p] < min_dist) {
                min_dist = T[p];
                p_star = p;
            }
        }
        
        // Update M[f] <- p*
        M[f] = p_star;

        // Update T[p*] <- T[p*] + 1
        T[p_star]++;
    }

    return M;
}

// Filtered Vamana Indexing Algorithm
Graph filtered_vamana_indexing(const Dataset &P, vector<vector<int>> F_x, double a, int L, int R) {
    int n = (int)P.size();

    // Initialize G to an empty graph
    Graph G; G.reserve(n);

    // Let s denote the medoid of P
    int s = medoid(P);

    // Let st(f) denote the start node of filter label f for every f \in F
    unordered_map<int, int> st = find_medoid(P, F_x, 1);

    // Let σ denote a random permutation of |n|
    vector<int> sigma = random_permutation(n);

    // Let F_x be the label-set for every x \in P

    // Foreach i \in |n| do
    for (int i = 0; i < n; i++) {
        int s_i = sigma[i];

        // Let S_F_σ_(i) = {st(f) | f \in F_x_σ_(i)}
        set<int> S_F_i;
        for (int f = 0; f < FILTERS; f++) {
            if (F_x[s_i][f] == 1) S_F_i.insert(st[f]);
        }

        // Let [{}, V_F_x_σ_(i)] = filtered_greedy_search(G, S_F_σ_(i), p_x_σ_(i), 0, L, F_x_σ_(i))
        pair<vector<int>, vector<int>> result = greedy_search(G, G[s_i], P[s_i], 0, L);
        vector<int> V_F_i = result.second;

        //? V = V U V_F_x_σ_(i)

        // G = filtered_robust_pruning(G, p_x_σ_(i), V_F_x_σ_(i), a, R, F_x_σ_(i))
        G = robust_pruning(G, G[s_i], V_F_i, a, R);

        // Foreach j \in N_out(σ(i)) do
        for (int j : G[s_i]->out_neighbours) {

            // Update N_out(j) <- N_out(j) U {σ(i)}
            G[j]->out_neighbours.insert(s_i);

            // If |N_out(j)| > R then
            if (G[j]->out_neighbours.size() > (size_t)R) {
                // G = filtered_robust_pruning(G, j, N_out(j), a, R, F_x_σ_(i))
            }
        }
    }

    return G;
}

Graph stiched_vamana_indexing(const Dataset &P, vector<vector<int>> F_x, double a, int L_small, int R_small, int R_stiched) {
    
    // Initialize G = (V, E) to an empty graph
    Graph G;
    G.reserve(P.size());

    // Let F_x be the label-set for every x \in P

    // Let P_f be the set of points with label f \in F
    vector<unordered_set<int>> P_f(FILTERS);
    for (size_t i = 0; i < P.size(); i++) {
        for (int f = 0; f < FILTERS; f++) {
            if (F_x[i][f] == 1) P_f[f].insert(i);
        }
    }

    // foreach f \in F do
    for (int f = 0; f < FILTERS; f++) {
        // Let G_f = vamana_indexing(P_f, a, L_small, R_small)
        Graph G_f = vamana_indexing(P, a, L_small, R_small);
    }

    // foreach v \in V(G_f) do
    for (auto &v : G) {
        // Let G = robust_pruning(G, v, V(G_f), a, R_stiched, F)
    }
    return G;
}

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
        Graph_Node node = create_graph_node(p);
        add_node_to_graph(G, node);
    }
    
    // Select up to R unique random neighbours
    int i = 0;
    for (auto &node : G) {
        
        set<int> random_indices;

        while (random_indices.size() < (size_t)R) {
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
        pair<vector<int>, vector<int>> result = greedy_search(G, s, p_d, L, L);

        vector<int> L = result.first; vector<int> V = result.second;

        // Run robust_prune to update out-neighbours of s_i
        G = robust_pruning(G, p, V, a, R);

        // For all points j in N_out(σ(i)) do
        for (auto j : p->out_neighbours) {
            // |N_out(j) U {σ(i)}|
            vector<int> N_out_j_p(G[j]->out_neighbours.begin(), G[j]->out_neighbours.end());
            N_out_j_p.push_back(i);

            // If |N_out(σ(i)) U {σ(i)}| > R then
            if (N_out_j_p.size() > (size_t)R) {

                // Run robust_prune to update out-neighbours of j
                G = robust_pruning(G, G[j], N_out_j_p, a, R);
            } else {
                
                // Add σ(i) to out-neighbours of j
                G[j]->out_neighbours.insert(i);
            }
        }
    }
    
    // Return the graph
    return G;
}
