#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <random>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

typedef long double data_t;

typedef vector<data_t> Data;

typedef vector<Data> Dataset;

struct graph_node {
    Data data;
    unordered_set<struct graph_node *> out_neighbours;
};

typedef struct graph_node* Graph_Node;

typedef unordered_set<Graph_Node> Node_Neighbours;

typedef vector<Graph_Node> Graph;

/* Graph functions */

Graph_Node create_graph_node(Data &data);
void add_node_to_graph(Graph &graph, Graph_Node &node);
void add_edge_to_graph(Graph_Node &from, const Graph_Node &to);
Graph_Node find_node_in_graph(Graph &graph, const Data &data);
void print_graph(Graph &graph);
void print_out_neighbours(Graph_Node &node);

/* Helper functions */

long double euclidean_distance(const Data &d1, const Data &d2);
vector<int> random_permutation(int n);
Dataset get_data(Graph &graph);
Data random_query(int dim);
Dataset random_dataset(int n, int dim);
void print_results(const Dataset &dataset, const Data &query, const vector<Data> &expected_neighbors,\
                   const Graph &result, const vector<pair<Data, data_t>> &distances);
void check_results_manually(const Dataset &dataset, const Data &query, const Graph &result,\
                            int k, Dataset expected_neighbors_g);

/* Greedy Search | s start node, q query, k result size, L search list size */

Graph L_m_V(Graph &L, Graph &V);
pair<Graph,Graph> greedy_search(Graph_Node s, Data q, int k, int L);

/* Robust Pruning */

Graph robust_pruning(Graph &G, Data &p, Dataset &V, double a, int R);

/* Vamana Indexing Algorithm */

int medoid(Dataset &P);
Graph vamana_indexing(Dataset &P, double a, int L, int R);

/* File Reading */

Dataset bvecs_read(string file_name);
Dataset fvecs_read(string file_name);
Dataset ivecs_read(string Filename_s);
