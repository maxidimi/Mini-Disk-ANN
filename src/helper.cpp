#include "../include/header.h"

// Computes the Euclidean distance between two data points

euclidean_t euclidean_distance(const Data &d1, const Data &d2) {
    if (d1.size() != d2.size()) {
        cerr << "Data points have different dimensions\n";
        exit(1);
    }

    size_t dim = d1.size();
    euclidean_t distance = 0.0L;
    for (size_t i = 0; i < dim; i++) {
        distance += (d2[i] - d1[i]) * (d2[i] - d1[i]);
    }
    
    //distance = sqrt(distance);
    
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

// Given a graph node list, returns the corresponding dataset
Dataset get_data(const Graph &graph) {
    Dataset data; data.reserve(graph.size());
    for (const auto &node : graph) {
        data.push_back(node->data);
    }
    return data;
}

// Generates a random query of the given dimension
Data random_query(int dim) {
    Data query;

    for (int i = 0; i < dim; i++) {
        query.push_back((data_t)rand() / RAND_MAX);
    }

    return query;
}

// Generates a random dataset of the given size and dimension
Dataset random_dataset(int n, int dim) {

    Dataset dataset;
    for (int i = 0; i < n; i++) {
        Data data;
        for (int j = 0; j < dim; j++) {
            data.push_back((data_t)rand() / RAND_MAX);
        }
        dataset.push_back(data);
    }
    return dataset;
}

// Checks the results of the Greedy search manually
void check_results(const Dataset &dataset, Data query, const Graph &result, int k, vector<int> expected_neighbors_g) {
    
    // Get the expected nearest neighbors (if not provided)
    vector<Data> expected_neighbors; expected_neighbors.reserve(k);
    if (expected_neighbors_g.empty()) {
        // Calculate the Euclidean distances of each point from the query
        vector<pair<Data, euclidean_t>> distances; distances.reserve(dataset.size());
        for (const auto &data : dataset) {
            euclidean_t dist = euclidean_distance(data, query);
            distances.emplace_back(data, dist);
        }
        
        // Sort distances in ascending order
        sort(distances.begin(), distances.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

        for (size_t i = 0; i < static_cast<size_t>(k); i++) {
            expected_neighbors.push_back(distances[i].first);
        }
    } else {
        // Here expected_neighbors_g is the groundtruth that contains the indices of the expected neighbors.
        for (const auto &index : expected_neighbors_g) {
            expected_neighbors.push_back(dataset[index]);
        }
    }

    // Check if the number of neighbors found matches k
    if (static_cast<int>(result.size()) != k) {
        cerr << " || Size of neighbours list found doesn't match k. Found: " << result.size() << " Expected: " << k << ".\n";}
    int foundC = 0;
    
    // Check if the neighbor is in the expected neighbors
    for (const auto &node : result) {
        if (node == nullptr) {
            cerr << "Error: nullptr\n";
            continue;
        }
        bool found = false;
        for (const auto &expected : expected_neighbors) {
            if (node->data == expected) {
                found = true;
                break;
            }
        }
        if (found) {foundC++;}
    }
    cout << " || Number of neighbours found in expected neighbors: " << foundC << "/" << k << ".\n";
    cout << " || Recall@k: " << (double)(100*foundC/k) << "%." << endl;
    cout << "=======================================================================================\n";
}

void time_elapsed(clock_t start, string message) {
    clock_t end = clock();
    double elapsed_seconds = double(end - start) / CLOCKS_PER_SEC;

    double minutes = static_cast<int>(elapsed_seconds) / 60.0;
    
    cout << fixed << setprecision(2);
    cout << " || Total time taken for " << message << ": " << elapsed_seconds <<\
     " seconds (= " << minutes << " minutes)" << endl;
}

Graph_Node find_min_dist(const Graph &G, const Data &q) {
    euclidean_t min_dist = numeric_limits<euclidean_t>::max();
    Graph_Node p_star = nullptr;
    for (const auto &p : G) {
        euclidean_t dist = euclidean_distance(p->data, q);
        if (dist < min_dist) {
            min_dist = dist;
            p_star = p;
        }
    }
    return p_star;
}
