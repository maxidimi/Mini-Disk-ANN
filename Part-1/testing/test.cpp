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
    for (double i = 0; i < 10; i++) {
        Data data = {i, i+1, i+2};
        P.push_back(data);
    }
    TEST_CHECK(P.size() == 10);
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

void test_euclidean_distance(void){
    //Test for 1D
    Data d1 = {20};
    Data d2 = {10};
    TEST_CHECK(fabs(euclidean_distance(d1, d2) - 10) < 1e-6);
    
    //Test for 2D
    d1 = {2, 5};
    d2 = {3, 4};
    TEST_CHECK(fabs(euclidean_distance(d1, d2) - sqrt(2)) < 1e-6);

    //Test for 3D
    d1 = {3, 6, 9};
    d2 = {2, 4, 6};
    TEST_CHECK(fabs(euclidean_distance(d1, d2) - sqrt(14)) < 1e-6);

    //Test for 4D
    d1 = {3, 6, 9, 12};
    d2 = {2, 4, 6, 8};
    TEST_CHECK(fabs(euclidean_distance(d1, d2) - sqrt(30)) < 1e-6);

}

void test_random_permutation(void){
    int n = 25;
    list<int> perm = random_permutation(n);
    TEST_CHECK(perm.size() == static_cast<std::list<int>::size_type>(n));

    n = 10;
    list<int> perm1 = random_permutation(n);
    list<int> perm2 = random_permutation(n);
    TEST_CHECK(perm1 != perm2);  
    
}



TEST_LIST = {
    { "test_create_graph_node", test_create_graph_node },
    { "test_example", test_example },
    { "test_knn", test_knn },
    {"test_euclidean_distance", test_euclidean_distance},
    {"test_random_permutation", test_random_permutation},
    { 0 }
};