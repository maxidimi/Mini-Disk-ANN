#include "../include/header.h"
#include "../include/acutest.h"
#include <exception>
#include <stdexcept>
#include <string>
// test files in src

void test_create_graph_node(void) {
    Data data = {1, 2, 3};
    Graph_Node node = create_graph_node(data);
    TEST_CHECK(node->data == data);
    TEST_CHECK(node->out_neighbours.size() == 0);
    TEST_CHECK(node->in_neighbours.size() == 0);
}

void test_example(void) {
    TEST_CHECK(1 == 1);
}

// Test results of vamana indexing
void test_knn(void) {
    // Create a dataset
    Dataset P;
    for (int i = 0; i < 10; i++) {
        Data data = {i, i+1, i+2};
        P.push_back(data);
    }
    TEST_CHECK(G.size() == 10);
    /*// Create a graph
    Graph G = vamana_indexing(P, 2, 3, 2);
    
    // Find the node closest to the query
    Data q = {1, 2, 3};

    Graph_Node s = create_graph_node(q);
    Dataset V; list<Graph_Node> V_list;
    Dataset result = greedy_search(G, s, q, 1, 3, V_list);

    list<double> distances;
    for (Data d : result) {
        distances.push_back(euclidean_distance(d, q));
    }
    // Find the minimum distance
    double min_distance = *min_element(distances.begin(), distances.end());*/
}

TEST_LIST = {
    { "test_create_graph_node", test_create_graph_node },
    { "test_example", test_example },
    { "test_knn", test_knn },
    { 0 }
};