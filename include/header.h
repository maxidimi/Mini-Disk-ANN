#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

#define FILTERS 4

using namespace std;

typedef float data_t;

typedef float euclidean_t;

typedef vector<data_t> Data;

typedef vector<Data> Dataset;

typedef set<int> Filters;

struct graph_node {
    Data data;
    unordered_set<int> out_neighbours;
    int indx;
};

typedef struct graph_node* Graph_Node;

typedef unordered_set<int> Node_Neighbours;

typedef vector<Graph_Node> Graph;

/* Graph functions */

Graph_Node create_graph_node(const Data &data, int R);
void add_node_to_graph(Graph &graph, Graph_Node &node);
void add_edge_to_graph(Graph_Node &from, int to);
Graph_Node find_data_in_graph(Graph &graph, const Data &data);

/* Helper functions */
euclidean_t euclidean_distance(const Data &d1, const Data &d2);
vector<int> random_permutation(size_t n);
Dataset get_data(const Graph &graph);
Data random_query(int dim);
Dataset random_dataset(int n, int dim);
void check_results(const Dataset &dataset, Data query, const Graph &result, int k, vector<int> expected_neighbors_g);
void time_elapsed(clock_t start, string message);
Graph_Node find_min_dist(const Graph &G, const Data &q);

/* Greedy Search | s start node, q query, k result size, L search list size */
void L_m_V(const Graph &L, const Graph &V, Graph &LV);
pair<Graph,Graph> greedy_search(const Graph &G, Graph_Node s, Data q, int k, int L);

/* Robust Pruning */
Graph robust_pruning(Graph &G, Data &p, Dataset &V, double a, int R, Graph_Node &s);

/* Vamana Indexing Algorithm */
int medoid(const Dataset &P);
Graph vamana_indexing(const Dataset &P, double a, int L, int R);

/* File Reading */
Dataset bvecs_read(string file_name);
Dataset fvecs_read(string file_name);
vector<vector<int>> ivecs_read(string Filename_s);
void store_graph(const Graph &G, string filename);
Graph read_graph(string filename);
