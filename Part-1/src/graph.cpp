#include "../include/header.h"

// Creates a graph node
Graph_Node create_graph_node(Data data) {
    Graph_Node node = new graph_node;
    node->data = data;
    return node;
}

// Adds a node to the graph
void add_node_to_graph(Graph &graph, Graph_Node node) {
    graph.push_back(node);
}

// Removes a node from the graph
void remove_node_from_graph(Graph &graph, Graph_Node node) {
    graph.remove(node);
}

// Adds an edge to the graph
void add_edge_to_graph(Graph_Node from, Graph_Node to) {
    // Check if edge already exists
    for (auto i : from->out_neighbours) {
        if (i == to) {
            return;
        }
    }
    from->out_neighbours.push_back(to);
    to->in_neighbours.push_back(from);
}

// Finds a node in the graph
Graph_Node find_node_in_graph(Graph &graph, Data data) {
    for (auto i : graph) {
        if (i->data == data) {
            return i;
        }
    }
    return nullptr;
}

// Prints the graph
void print_graph(Graph &graph) {
    cout << "Graph:\n\t";
    for (auto i : graph) {
        cout << "( ";
        for (auto j : i->data) {
            cout << j << " ";
        } cout << ") ";
    } cout << endl;
}

// Prints the out neighbours of a node
void print_out_neighbours(Graph_Node node) {
    cout << "\nOut-neighbours of node\n\t";
    for (auto i : node->out_neighbours) {
        cout << "( ";
        for (auto j : i->data) {
            cout << j << " ";
        } cout << ") ";
    } cout << endl;
}

// Prints the in neighbours of a node
void print_in_neighbours(Graph_Node node) {
    cout << "\nIn-neighbours of node\n\t";
    for (auto i : node->in_neighbours) {
        cout << "( ";
        for (auto j : i->data) {
            cout << j << " ";
        } cout << ")";
    } cout << endl;
}
