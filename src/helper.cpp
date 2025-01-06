#include "../include/header.h"

// Computes the Euclidean distance between two data points
euclidean_t euclidean_distance(const Data &d1, const Data &d2) {
    size_t dim = d1.size();
    euclidean_t distance = 0.0L;
    for (size_t i = 0; i < dim; i++) {
        distance += (d2[i] - d1[i]) * (d2[i] - d1[i]);
    }

    return distance;
}

// Generates a random permutation of integers from 0 to n-1
vector<int> random_permutation(size_t n) {
    vector<int> perm(n);
    for (size_t i = 0; i < n; i++) {
        perm[i] = int(i);
    }

    random_device rd; mt19937 g(rd());
    shuffle(perm.begin(), perm.end(), g);
    
    return perm;
}

// Finds the node in L with the minimum distance to q
int find_min_dist(const Graph G, vector<int> L, Data q) {
    int min_indx = -1;
    double min_dist = numeric_limits<double>::max();
    for (const auto &node : L) {
        double dist = euclidean_distance(G[node]->data, q);
        if (dist < min_dist) {
            min_dist = dist;
            min_indx = node;
        }
    }
    
    return min_indx;
}

// Checks the results of the Greedy search manually
double check_results(const vector<int> &result, int k, vector<int> expected_neighbors, bool print) {
    int n_size = (int)expected_neighbors.size();
    int r_size = (int)result.size();

    // Check if the number of neighbors found matches k (or the expected neighbors)
    if (r_size != k && r_size != n_size) return 0.0;
    
    // Check if the results neighbor is in the expected neighbors
    int foundC = 0;
    for (const auto &r : result) {
        if (find(expected_neighbors.begin(), expected_neighbors.end(), r) != expected_neighbors.end()) {
            foundC++;
        }
    }
    double recall;
    if (n_size == 0) {
        recall = 100.0;
    } else {
        recall = (double)(100*foundC/n_size);
    }
    if (print) {
        cout << " || Number of neighbours found in expected neighbors: " << foundC << "/" << n_size << ".\n";
        cout << " || Recall@" << k << " : " << recall << "%." << endl;
        cout << "=======================================================================================\n";
    }
    
    return recall;
}

// Prints the time taken for a given operation
void time_elapsed(high_resolution_clock::time_point start, const string& message) {
    auto end = high_resolution_clock::now();
    duration<double> elapsed_seconds = end - start;

    double total_seconds = elapsed_seconds.count();
    double minutes = total_seconds / 60.0;

    cout << fixed << setprecision(2);
    cout << " || Total time taken for " << message << ": " 
         << total_seconds << " seconds (= " << minutes << " minutes)" << endl;
}

// Generates a random query of the given dimension
Data random_query(int dim) {
    Data query;

    for (int i = 0; i < dim; i++) {
        query.push_back(static_cast<data_t>(rand()) / static_cast<data_t>(RAND_MAX));
    }

    return query;
}

// Generates a random dataset of the given size and dimension
Dataset random_dataset(int n, int dim) {
    Dataset dataset; dataset.reserve(n);
    for (int i = 0; i < n; i++) {
        Data data;
        for (int j = 0; j < dim; j++) {
            data.push_back(static_cast<data_t>(rand()) / static_cast<data_t>(RAND_MAX));
        }
        dataset.push_back(data);
    }
    return dataset;
}
