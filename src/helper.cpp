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
vector<int> random_permutation(int n) {
    vector<int> perm(n);
    for (int i = 0; i < n; i++) {
        perm[i] = i;
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

// Prints the results of the Vamana indexing and Greedy search
void print_results(const Dataset &dataset, const Data &query, const vector<Data> &expected_neighbors,\
                     const Graph &result, const vector<pair<Data, euclidean_t>> &distances) {
    // Print the distances with corresponding points
    cout << "\n\nDistances from query point (" << query[0] << ", " << query[1] << "):\n";
    for (const auto &[point, distance] : distances) {
        cout << "Point: (" << point[0] << ", " << point[1] << ") - Distance: " << distance << endl;
    }
    cout << "\nExpected nearest neighbors:\n";
    for (const auto &neighbor : expected_neighbors) {
        cout << "\tPoint: (" << neighbor[0] << ", " << neighbor[1] << ")\n";
    }
    cout << "Greedy search results:\n";
    for (const auto &node : result) {
        if (node == nullptr) {
            cout << "\tPoint: (nullptr)\n";
        } else 
        {cout << "\tPoint: (" << node->data[0] << ", " << node->data[1] << ")\n";}
    }
}

// Checks the results of the Greedy search manually
void check_results(const Dataset &dataset, const Data &query, const Graph &result, int k, vector<int> expected_neighbors_g) {
    
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

    if (static_cast<int>(result.size()) == k) cout << " || Size of neighbours list found matches k.\n";
    else {cerr << " || Size of neighbours list found doesn't match k. Found: " << result.size() << " Expected: " << k << ".\n";}
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
