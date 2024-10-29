#include "../include/header.h"

// Creates a graph node
Graph_Node create_graph_node(Data &data) {
    Graph_Node node = new graph_node;
    node->data = data;
    return node;
}

// Adds a node to the graph
void add_node_to_graph(Graph &graph, Graph_Node &node) {
    graph.push_back(node);
}

// Adds an edge to the graph
void add_edge_to_graph(Graph_Node &from, const Graph_Node &to) {
    from->out_neighbours.insert(to);
}

// Finds a node in the graph
Graph_Node find_node_in_graph(Graph &graph, const Data &data) {
    auto it = find_if(graph.begin(), graph.end(), [&data](Graph_Node node) {
        return node->data == data;
    });
    if (it != graph.end()) {
        return *it;
    }
    return nullptr;
}

// Prints the graph
void print_graph(Graph &graph) {
    cout << "Graph:\n\t";
    for (const auto &i : graph) {
        cout << "( ";
        for (const auto &j : i->data) {
            cout << j << " ";
        } cout << ") ";
    } cout << endl;
}

// Prints the out neighbours of a node
void print_out_neighbours(Graph_Node &node) {
    cout << "\nOut-neighbours of node: (";
    for (const auto &i : node->data) {
        cout << i << ", ";
    } cout << ")\n\t";
    for (const auto &i : node->out_neighbours) {
        cout << "( ";
        for (const auto &j : i->data) {
            cout << j << " ";
        } cout << ") ";
    } cout << endl;
}
