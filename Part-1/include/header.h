#include <iostream>
#include <list>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <iterator>
#include <fstream>
#include <cassert>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <set>
#include <exception>
#include <string>
#include <cstdio>

using namespace std;

typedef vector<double> Data;

typedef list<Data> Dataset;

struct graph_node {
    Data data;
    list<struct graph_node *> out_neighbours;
    list<struct graph_node *> in_neighbours;
};

typedef struct graph_node* Graph_Node;

typedef list<Graph_Node> Graph;

/* Graph functions */

Graph_Node create_graph_node(Data data);
void add_node_to_graph(Graph &graph, Graph_Node node);
void remove_node_from_graph(Graph &graph, Graph_Node node);
void add_edge_to_graph(Graph_Node from, Graph_Node to);
Graph_Node find_node_in_graph(Graph &graph, Data data);
void print_graph(Graph &graph);
void print_out_neighbours(Graph_Node node);
void print_in_neighbours(Graph_Node node);

/* Helper functions */

double euclidean_distance(Data d1, Data d2);
list<int> random_permutation(int n);
void shuffle_list (list<int>& el_list);
Data getElementAtIndex(Dataset& mylist, size_t index);
void changeElementAtIndex(Dataset& mylist, size_t index, Data data);
Dataset get_data(list<Graph_Node> graph);
list<Graph_Node> L_m_V(list<Graph_Node> L, list<Graph_Node> V);
Graph_Node get_node_at_index(Graph& graph, Dataset P, int index);
Data random_query(int dim);
Dataset random_dataset(int n, int dim);

/* Greedy Search | s start node, q query, k result size, L search list size */

pair<list<Graph_Node>,list<Graph_Node>> greedy_search(Graph_Node s, Data q, int k, int L);

/* Robust Pruning */

Graph robust_pruning(Graph G, Data p, Dataset V, double a, int R);

/* Vamana Indexing Algorithm */

Data medoid(Dataset P);
Graph vamana_indexing(Dataset P, double a, int L, int R);

vector<vector<uint8_t>> bvecs_read(char* file_name);
vector<vector<float>> fvecs_read(char* file_name);
