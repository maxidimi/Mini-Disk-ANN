#include "../include/header.h"

// Creates a graph node
Graph_Node create_graph_node(const Data &data) {
    Graph_Node node = new graph_node;
    node->data = data;
    
    return node;
}

// Adds a node to the graph
void add_node_to_graph(Graph &graph, Graph_Node &node) {
    graph.push_back(node);
    node->indx = graph.size() - 1;
}

// Adds an edge to the graph
void add_edge_to_graph(Graph_Node &from, int to) {
    if (from->indx != to) // Avoid self-loops
        from->out_neighbours.insert(to);
}

// Finds a node in the graph
Graph_Node find_data_in_graph(Graph &graph, const Data &data) {
    auto it = find_if(graph.begin(), graph.end(), [&data](Graph_Node node) {
        return node->data == data;
    });
    if (it != graph.end()) {
        return *it;
    }
    return nullptr;
}
