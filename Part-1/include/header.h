#include <iostream>
#include <list>

using namespace std;

struct data {
    int x;
    int y;
};

typedef struct data* Data;

struct graph_node {
    Data data;
    list<struct graph_node *> out_neighbours;
    list<struct graph_node *> in_neighbours;
};

typedef struct graph_node* Graph_Node;

// Graph functions
Graph_Node create_graph_node(Data data);
void add_node_to_graph(list<Data> &graph, Graph_Node node);
void remove_node_from_graph(list<Data> &graph, Graph_Node node);
void add_to_edge_graph(list<Data> &graph, Graph_Node from, Graph_Node to);
Graph_Node find_node_in_graph(list<Data> &graph, Data data, int compare(Data, Data));

// Helper functions
int compare(Data d1, Data d2);
