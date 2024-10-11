#include "../include/header.h"

//
Graph_Node create_graph_node(Data data) {
    Graph_Node node = new graph_node;
    node->data = data;
    return node;
}

//
void add_node_to_graph(list<Data> &graph, Graph_Node node) {
    graph.push_back(node->data);
}

//
void remove_node_from_graph(list<Data> &graph, Graph_Node node) {
    graph.remove(node->data);
}

//
void add_to_edge_graph(list<Data> &graph, Graph_Node from, Graph_Node to) {
    from->out_neighbours.push_back(to);
    to->in_neighbours.push_back(from);
}

//
Graph_Node find_node_in_graph(list<Data> &graph, Data data, int compare(Data, Data)) {
    for (auto i : graph) {
        if (compare(i, data)) {
            return create_graph_node(i);
        }
    }
    return NULL;
}
