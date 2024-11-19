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

using namespace std;

typedef float data_t;

typedef float euclidean_t;

typedef vector<data_t> Data;

typedef vector<Data> Dataset;

struct graph_node {
    Data data;
    unordered_set<int> out_neighbours;
    int indx;
};

typedef struct graph_node* Graph_Node;

typedef unordered_set<int> Node_Neighbours;

typedef vector<Graph_Node> Graph;

/* Graph functions */
Graph_Node create_graph_node(const Data &data);
void add_node_to_graph(Graph &graph, Graph_Node &node);
void add_edge_to_graph(Graph_Node &from, int to);
Graph_Node find_data_in_graph(Graph &graph, const Data &data);

/* Helper functions */
euclidean_t euclidean_distance(const Data &d1, const Data &d2);
vector<int> random_permutation(size_t n);
int find_min_dist(const Graph G, vector<int> L, Data q);
double check_results(const Dataset &dataset, Data query, const vector<int> &result, int k, vector<int> expected_neighbors_g, bool print);
void time_elapsed(clock_t start, string message);
Data random_query(int dim);
Dataset random_dataset(int n, int dim);

/* Greedy Search | s start node, q query, k result size, L search list size */
void L_m_V(const vector<int> &L, const vector<int> &V, vector<int> &LV);
pair<vector<int>, vector<int>> greedy_search(const Graph &G, list<Graph_Node> S, Data q, int k, int L_s,float filter, float fq);
pair<vector<int>, vector<int>> greedy_search_part1(const Graph &G, Graph_Node s, Data q, int k, int L_s);
/* Robust Pruning */
Graph robust_pruning(Graph &G, Graph_Node &p_node, vector<int> &V, double a, int R);

/* Vamana Indexing Algorithm */
int medoid(const Dataset &P);
Graph vamana_indexing(const Dataset &P, double a, int L, int R);

/* File Reading and Storing */
pair<Dataset, vector<int>> read_sigmod_queries(string file_name);
pair<Dataset, vector<int>> read_sigmod_dataset(string file_name);
Dataset bvecs_read(string file_name);
Dataset fvecs_read(string file_name);
vector<vector<int>> ivecs_read(string Filename_s);
void store_graph(const Graph &G, string filename);
Graph read_graph(string filename);
