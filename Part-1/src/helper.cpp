#include "../include/header.h"

// Computes the Euclidean distance between two data points
double euclidean_distance(Data d1, Data d2) {
    auto distance = 0.0;
    for (uint i = 0; i < d1.size(); i++) {
        distance += pow(d1[i] - d2[i], 2);
    }
    
    distance = sqrt(distance);

    return distance;
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
void shuffle_list (list<int>& el_list) {
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
Data getElementAtIndex(Dataset& mylist, size_t index) {
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
void changeElementAtIndex(Dataset& mylist, size_t index, Data data) {
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
Dataset get_data(list<Graph_Node> graph) {
    Dataset data;
    for (auto node : graph) {
        data.push_back(node->data);
    }
    return data;
}

// L\V operation, returns the elements in L that are not in V
list<Graph_Node> L_m_V(list<Graph_Node> L, list<Graph_Node> V) {
    list<Graph_Node> result;
    for (auto i : L) {
        if (find_node_in_graph(V, i->data) == nullptr) {
            result.push_back(i);
        }
    }
    return result;
}

// Wrapper function to get the node in the graph with the given data
Graph_Node get_node_at_index(Graph& graph, Dataset P, int index) {
    auto data = getElementAtIndex(P, index);
    return find_node_in_graph(graph, data);
}
