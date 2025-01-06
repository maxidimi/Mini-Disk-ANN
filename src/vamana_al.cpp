#include "../include/header.h"

// Find Medoid
unordered_map<int, int> find_medoid(const Dataset &P, vector<int> C, int threshold, vector<int> F) {
    size_t n = P.size();
    size_t f_size = F.size();

    // Initialize M be an empty map
    unordered_map<int, int> M; M.reserve(f_size);

    // Initialize T to a zero map, T is intended as a counter
    unordered_map<int, int> T; T.reserve(n);
    for (size_t i = 0; i < n; i++) T[i] = 0;

    // Foreach f \in F do
    for (auto f : F) {
        // Let P_f denote the ids of all points matching filter f
        unordered_set<int> P_f;
        for (size_t i = 0; i < n; i++) {
            if (C[i] == f) P_f.insert(i);
        }

        // Let R_F <- τ randomly sampled data point ids from P_f
        vector<int> R_f(P_f.begin(), P_f.end());
        shuffle(R_f.begin(), R_f.end(), mt19937(random_device()()));

        // Let size of R_f be min{τ, |P_f|} and keep only the first threshold elements
        if (threshold < (int)R_f.size()) R_f.resize(threshold);

        // p* <- argmin_{p \in R_F} T[p]
        int p_star = -1;
        int min_counter = numeric_limits<int>::max();
        for (const auto &p : R_f) {
            if (T[p] <= min_counter) {
                min_counter = T[p];
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

// Returns the subgraph indexed graph G_f for filter f /in F
Graph subgraph_filtered(const Dataset &P, double a, int L, int R, int st, int F) {
    int n = static_cast<int>(P.size());
    
    vector<int> sigma = random_permutation(n);

    Graph G; G.reserve(n);

    vector<int> F_x(n, F);

    for (const auto &p: P) {
        Graph_Node node = create_graph_node(p);
        add_node_to_graph(G, node);
    }

    for (int i = 0; i < n; ++i) {
        int s_i = sigma[i];

        // Let S_F_σ_(i) = {st(f) | f ∈ F_x_σ_(i)}, but here there is only one f
        vector<int> S_F_i = {st};

        // Run filtered_greedy_search
        pair<vector<int>, vector<int>> result = filtered_greedy_search(G, S_F_i, P[s_i], 0, L, F_x, {F_x[s_i]});
        vector<int> V_F_i = result.second;

        // Run filtered_robust_pruning
        G = filtered_robust_pruning(G, G[s_i], V_F_i, a, R, F_x);

        for (int j : G[s_i]->out_neighbours) {
            G[j]->out_neighbours.insert(s_i);

            // Check and prune if necessary
            if (G[j]->out_neighbours.size() > static_cast<size_t>(R)) {
                vector<int> N_out_j(G[j]->out_neighbours.begin(), G[j]->out_neighbours.end());
                G = filtered_robust_pruning(G, G[j], N_out_j, a, R, F_x);
            }
        }
    }

    return G;
}

// Filtered Vamana Indexing Algorithm
Graph filtered_vamana_indexing(const Dataset &P, vector<int> F_x, double a, int L, int R, vector<int> F) {
    int n = static_cast<int>(P.size());
    size_t f_size = F.size();

    // Add the nodes to the graph G
    Graph G; G.reserve(n);

    // Let st(f) denote the start node of filter label f for every f ∈ F
    unordered_map<int, int> st = find_medoid(P, F_x, 1, F);
    
    // Let σ denote a random permutation of |n|
    vector<int> sigma = random_permutation(n);

    // Let P_f be the set of points with label f \in F
    unordered_map<int, Dataset> P_f; P_f.reserve(f_size);

    // ind_corr matches every point in P to its index in the corresponding graph G[f], f \in F
    vector<int> ind_corr(n);

    // counters[f] keeps track of the number of points with filter f that have already been added to the stitched graph
    vector<int> counters(f_size, 0);
    
    // Let F_f be the set of indices with label f \in F
    vector<vector<int>> F_f(f_size);

    for (int i = 0; i < n; i++) {
        P_f[F_x[i]].push_back(P[i]);
        F_f[F_x[i]].push_back(i);
        ind_corr[i] = counters[F_x[i]]++;
        Graph_Node node = create_graph_node(P[i]);
        add_node_to_graph(G, node);
    }

    // Parallelize the subgraph_filtered calls with openMP
    unordered_map<int, Graph> G_f; G_f.reserve(f_size);
    #pragma omp parallel for schedule(dynamic) num_threads(omp_get_max_threads())
    for (size_t i = 0; i < f_size; i++) {
        auto G_f_i = subgraph_filtered(P_f[F[i]], a, L, R, ind_corr[st[F[i]]], F[i]);
        #pragma omp critical
        G_f[F[i]] = G_f_i;
    }

    // Add the out-neighbours in G[f] to the stitched graph
    for (int i = 0; i < n; i++) {
        Graph_Node node = G_f[F_x[i]][ind_corr[i]];
        for (int j : node->out_neighbours) {
            add_edge_to_graph(G[i], F_f[F_x[i]][j]);
        }
    }

    // Release memory
    for (auto &g : G_f) {
        for (auto &node : g.second) {
            delete node;
        }
    }

    return G;
}

Graph stitched_vamana_indexing(const Dataset &P, vector<int> F_x, double a, int L, int R, vector<int> F) {
    size_t f_size = F.size();
    size_t p_size = P.size();
    
    // Initialize G = (V, E) to an empty graph, G_stitched as the stitched graph
    unordered_map<int, Graph> G; G.reserve(f_size);
    Graph G_stitched; G_stitched.reserve(p_size);

    // Let P_f be the set of points with label f \in F
    unordered_map<int, Dataset> P_f; P_f.reserve(f_size);

    // ind_corr matches every point in P to its index in the corresponding graph G[f], f \in F
    vector<int> ind_corr(p_size);

    // counters[f] keeps track of the number of points with filter f that have already been added to the stitched graph
    vector<int> counters(f_size, 0);
    
    // Let F_f be the set of indices with label f \in F
    vector<vector<int>> F_f(f_size);

    for (size_t i = 0; i < p_size; i++) {
        P_f[F_x[i]].push_back(P[i]);
        F_f[F_x[i]].push_back(i);
        ind_corr[i] = counters[F_x[i]]++;
        Graph_Node node = create_graph_node(P[i]);
        add_node_to_graph(G_stitched, node);
    }

    // Parallelize the vamana_indexing calls with openMP
    #pragma omp parallel for schedule(dynamic)
    for (size_t i = 0; i < f_size; i++) {
        auto G_f = vamana_indexing(P_f[F[i]], a, L, R);
        #pragma omp critical
        G[F[i]] = G_f;
    }

    // Add the out-neighbours in G[f] to the stitched graph
    for (size_t i = 0; i < p_size; i++) {
        Graph_Node node = G[F_x[i]][ind_corr[i]];
        for (int j : node->out_neighbours) {
            add_edge_to_graph(G_stitched[i], F_f[F_x[i]][j]);
        }
    }

    // Release memory
    for (auto &g : G) {
        for (auto &node : g.second) {
            delete node;
        }
    }

    return G_stitched;
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

    if (n == 1) return 0;

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

    if (n == 1) return G;
    
    // Select up to R unique random neighbours
    int i = 0;
    for (auto &node : G) {
        
        set<int> random_indices;

        while (random_indices.size() < (size_t)R && random_indices.size() < n - 1) {
            int random_idx = rand() % n;

            if (random_idx == i) continue;

            random_indices.insert(random_idx);
        }

        for (const auto &idx : random_indices) {
            add_edge_to_graph(node, idx);
        }i++;
    }

    // Let s denote the medoid of dataset P randomly
    int s_index = rand() % n;
    
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
