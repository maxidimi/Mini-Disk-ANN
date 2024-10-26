#include "../include/header.h"

// Computes the Euclidean distance between two data points
data_t euclidean_distance(const Data &d1, const Data &d2) {
    if (d1.size() != d2.size()) {
        cerr << "Data points have different dimensions\n";
        exit(1);
    }
    size_t dim = d1.size(); data_t distance = 0.0; 
    for (size_t i = 0; i < dim; i++) {
        distance += (d2[i] - d1[i]) * (d2[i] - d1[i]);
    }
    
    return sqrt(distance);
}

// Generates a random permutation of integers from 0 to n-1
vector<int> random_permutation(int n) {
    vector<int> perm(n);
    for (int i = 0; i < n; i++) {
        perm[i] = i;
    }

    shuffle(perm.begin(), perm.end(), mt19937(random_device()()));

    return perm;
}

// Returns the element at the given index in a list
Data get_element_at_index(Dataset &mylist, size_t index) {
    // Check if index is out of bounds
    if (index >= mylist.size()) {
        cerr << "Index is out of bounds\n";
        exit(1);
    }

    return mylist[index];
}

// Changes the element at the given index in a list
void change_element_at_index(Dataset &mylist, size_t index, Data &data) {
    // Check if index is out of bounds
    if (index >= mylist.size()) {
        cerr << "Index is out of bounds\n";
        exit(1);
    }

    mylist[index] = data;
}

// Given a graph node list, returns the corresponding dataset
Dataset get_data(Graph &graph) {
    Dataset data;
    for (const auto &node : graph) {
        data.push_back(node->data);
    }
    return data;
}

// L\V operation, returns the elements in L that are not in V
Graph L_m_V(Graph &L, Graph &V) {
    Graph result;
    for (const auto &node : L) {
        if (find(V.begin(), V.end(), node) == V.end()) {
            result.push_back(node);
        }
    }
    
    return result;
}

// Wrapper function to get the node in the graph with the given data
Graph_Node get_node_at_index(Graph &graph, Dataset &P, int index) {
    auto data = P[index];
    
    return find_node_in_graph(graph, data);
}

// Generates a random query of the given dimension
Data random_query(int dim) {
    Data query; srand((unsigned int)time(0));
    for (int i = 0; i < dim; i++) {
        query.push_back((data_t)rand() / RAND_MAX);
    }
    return query;
}

// Generates a random dataset of the given size and dimension
Dataset random_dataset(int n, int dim) {
    srand((unsigned int)time(0));
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
                     const Graph &result, const vector<pair<Data, data_t>> &distances) {
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
void check_results_manually(const Dataset &dataset, const Data &query, const Graph &result, int k, Dataset expected_neighbors_g) {
    // Calculate the Euclidean distances of each point from the query
    vector<pair<Data, data_t>> distances;
    for (const auto &data : dataset) {
        data_t dist = euclidean_distance(data, query);
        distances.emplace_back(data, dist);
    }
    
    // Sort distances in ascending order
    sort(distances.begin(), distances.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });
    
    // Get the expected nearest neighbors (if not provided)
    vector<Data> expected_neighbors;
    if (expected_neighbors_g.empty()) {
        for (size_t i = 0; i < (size_t)k; i++) {
            expected_neighbors.push_back(distances[i].first);
        }
    } else {
        expected_neighbors = expected_neighbors_g;
    }

    // Check if the number of neighbors found matches k
    if (result.size() == (size_t)k) cout << " || Number of neighbors is k.\n";
    else {cerr << " || Number of neighbors found is not k. Found: " << result.size() << " Expected: " << k << ".\n";}
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
    cout << " || Number of neighbors found in expected neighbors: " << foundC << ".\n";
}
