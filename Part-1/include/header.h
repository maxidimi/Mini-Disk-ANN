#include <iostream>
#include <list>
#include <vector>
#include <math.h>
#include <algorithm>
#include <random>

using namespace std;

typedef vector<int> Data;

typedef list<Data> Dataset;

struct graph_node {
    Data data;
    list<struct graph_node *> out_neighbours;
    list<struct graph_node *> in_neighbours;
};

typedef struct graph_node* Graph_Node;

typedef list<Graph_Node> Graph;

// Graph functions
Graph_Node create_graph_node(Data data);
void add_node_to_graph(Graph &graph, Graph_Node node);
void remove_node_from_graph(Graph &graph, Graph_Node node);
void add_to_edge_graph(Graph_Node from, Graph_Node to);
Graph_Node find_node_in_graph(Graph &graph, Data data);
void print_graph(Graph &graph);
void print_out_neighbours(Graph_Node node);
void print_in_neighbours(Graph_Node node);

// Helper functions
int euclidean_distance(Data d1, Data d2);
void random_permutation(vector<int> &sigma, Dataset P);

// Greedy Search
Dataset greedy_search(Graph_Node s, Data q, int k, int L);

// Robust Pruning
Graph robust_pruning(Graph G, Data p, Dataset V, int a, int R);

// Vamana Indexing Algorithm
Data medoid(Dataset P);
Graph vamana_indexing(Dataset P, int a, int L, int R);
