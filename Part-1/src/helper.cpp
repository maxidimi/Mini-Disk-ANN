#include "../include/header.h"

// Computes the Euclidean distance between two data points
double euclidean_distance(const Data &d1, const Data &d2) {
    size_t dim = d1.size(); auto distance = 0.0; 
    for (size_t i = 0; i < dim; i++) {
        distance += (d2[i] - d1[i]) * (d2[i] - d1[i]);
    }

    return sqrt(distance);
}

// Generates a random permutation of integers from 0 to n-1
list<int> random_permutation(int n) {
    list<int> perm;
    for (int i = 0; i < n; i++) {
        perm.push_back(i);
    }
    shuffle_list(perm);

    return perm;
}

// Shuffles the elements of a list
void shuffle_list (list<int> &el_list) {
    // Create a random number generator
    random_device rd;
    mt19937 g(rd());

    // Convert list to vector for shuffling
    vector<int> el_vector(el_list.begin(), el_list.end());

    // Shuffle the vector
    shuffle(el_vector.begin(), el_vector.end(), g);

    // Clear the original list and insert shuffled elements back
    el_list.clear();
    el_list.insert(el_list.begin(), el_vector.begin(), el_vector.end());
}

// Returns the element at the given index in a list
Data get_element_at_index(Dataset &mylist, size_t index) {
    // Check if index is out of bounds
    if (index >= mylist.size()) {
        throw out_of_range("Index is out of bounds");
    }

    // Create an iterator and advance it to the index
    auto it = mylist.begin();
    advance(it, index);

    // Return the element at the given index
    return *it;
}

// Changes the element at the given index in a list
void change_element_at_index(Dataset &mylist, size_t index, Data &data) {
    // Check if index is out of bounds
    if (index >= mylist.size()) {
        throw out_of_range("Index is out of bounds");
    }

    // Create an iterator and advance it to the index
    auto it = mylist.begin();
    advance(it, index);

    // Change the element at the given index
    *it = data;
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
    auto data = get_element_at_index(P, index);
    return find_node_in_graph(graph, data);
}

// Generates a random query of the given dimension
Data random_query(int dim) {
    Data query;
    for (int i = 0; i < dim; i++) {
        query.push_back((double)rand() / RAND_MAX);
    }
    return query;
}

// Generates a random dataset of the given size and dimension
Dataset random_dataset(int n, int dim) {
    Dataset dataset;
    for (int i = 0; i < n; i++) {
        Data data;
        for (int j = 0; j < dim; j++) {
            data.push_back((double)rand() / RAND_MAX);
        }
        dataset.push_back(data);
    }
    return dataset;
}
