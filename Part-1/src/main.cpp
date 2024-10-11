#include "../include/header.h"

int main() {
    list<Data> graph;

    Data d1 = new data;
    d1->x = 1;
    d1->y = 1;

    Data d2 = new data;
    d2->x = 2;
    d2->y = 2;
    
    Graph_Node n1 = create_graph_node(d1);
    Graph_Node n2 = create_graph_node(d2);
    add_node_to_graph(graph, n1);
    add_node_to_graph(graph, n2);

    add_to_edge_graph(graph, n1, n2);
    cout << "Graph" << endl;
    for (auto i : graph) {
        cout << i->x << " " << i->y << endl;
    } cout << "Out neighbours of n1" << endl;
    for (auto i : n1->out_neighbours) {
        cout << i->data->x << " " << i->data->y << endl;
    } cout << "In neighbours of n2" << endl;
    for (auto i : n2->in_neighbours) {
        cout << i->data->x << " " << i->data->y << endl;
    }

    remove_node_from_graph(graph, n1);
    cout << "Graph" << endl;
    for (auto i : graph) {
        cout << i->x << " " << i->y << endl;
    }

    auto n3 = find_node_in_graph(graph, d2, compare);
    cout << "Found node in graph: " << n3->data->x << " " << n3->data->y << endl;
    remove_node_from_graph(graph, n3);
        cout << "Graph" << endl;
    for (auto i : graph) {
        cout << i->x << " " << i->y << endl;
    }

    return 0;
}
