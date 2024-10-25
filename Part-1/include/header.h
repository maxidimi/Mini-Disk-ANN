#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <random>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>

using namespace std;

typedef float data_t;

typedef vector<float> Data;

typedef vector<Data> Dataset;

struct graph_node {
    Data data;
    list<struct graph_node *> out_neighbours;
};

typedef struct graph_node* Graph_Node;

typedef list<Graph_Node> Graph;

/* Graph functions */

Graph_Node create_graph_node(Data &data);
void add_node_to_graph(Graph &graph, Graph_Node node);
void add_edge_to_graph(Graph_Node from, Graph_Node to);
Graph_Node find_node_in_graph(Graph &graph, const Data &data);
void print_graph(Graph &graph);
void print_out_neighbours(Graph_Node node);

/* Helper functions */

data_t euclidean_distance(const Data &d1, const Data &d2);
vector<int> random_permutation(int n);
Data get_element_at_index(Dataset &mylist, size_t index);
void change_element_at_index(Dataset &mylist, size_t index, Data &data);
Dataset get_data(Graph &graph);
Graph L_m_V(Graph &L, Graph &V);
Graph_Node get_node_at_index(Graph &graph, Dataset &P, int index);
Data random_query(int dim);
Dataset random_dataset(int n, int dim);
void print_results(const Dataset &dataset, const Data &query, const vector<Data> &expected_neighbors,\
                   const Graph &result, const vector<pair<Data, double>> &distances);
void check_results_manually(const Dataset &dataset, const Data &query, const Graph &result,\
                            int k, Dataset expected_neighbors_g);

/* Greedy Search | s start node, q query, k result size, L search list size */

pair<Graph,Graph> greedy_search(Graph_Node s, Data q, int k, int L);

/* Robust Pruning */

Graph robust_pruning(Graph &G, Data &p, Dataset &V, double a, int R);

/* Vamana Indexing Algorithm */

Data medoid(Dataset &P);
Graph vamana_indexing(Dataset &P, double a, int L, int R);
=======
Data medoid(Dataset P);
Graph vamana_indexing(Dataset P, double a, int L, int R);

/* File Reading */

vector<vector<uint8_t>> bvecs_read(char* file_name);
vector<vector<float>> fvecs_read(char* file_name);
