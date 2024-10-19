#include "../include/header.h"
#include "../include/acutest.h"
#include <exception>
#include <stdexcept>
#include <string>
// test files in src

//Tests for Helper functions
void test_create_graph_node(void) {
    Data data = {1, 2, 3};
    Graph_Node node = create_graph_node(data);
    TEST_CHECK(node->data == data);
    TEST_CHECK(node->out_neighbours.size() == 0);
    TEST_CHECK(node->in_neighbours.size() == 0);
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

void test_shuffle_list(void){
    list<int> new_list;
    for (int i = 0; i < 10000; ++i) {
        new_list.push_back(i);
    }
    int list_size = new_list.size();
    shuffle_list(new_list);

    list<int> new_list2;
    shuffle_list(new_list2);
    
    //Testing if an empty list remains empty after shuffle  
    TEST_CHECK(new_list2.size()==0);
    //Testing if the first value of the list changed after shuffle
    TEST_CHECK(new_list.front() != 0);
    //Testing if the size of the value remains the same after shuffle
    TEST_CHECK(static_cast<size_t>(list_size) == new_list.size());
}

void test_euclidean_distance(void){
    
    Data d1 = {20};
    Data d2 = {10};
    //Test for 1D
    TEST_CHECK(fabs(euclidean_distance(d1, d2) - 10) < 1e-6);
    
    d1 = {2, 5};
    d2 = {3, 4};
    //Test for 2D
    TEST_CHECK(fabs(euclidean_distance(d1, d2) - sqrt(2)) < 1e-6);

    d1 = {3, 6, 9};
    d2 = {2, 4, 6};
    //Test for 3D
    TEST_CHECK(fabs(euclidean_distance(d1, d2) - sqrt(14)) < 1e-6);
    
    d1 = {3, 6, 9, 12};
    d2 = {2, 4, 6, 8};
    //Test for 4D
    TEST_CHECK(fabs(euclidean_distance(d1, d2) - sqrt(30)) < 1e-6);
}

void test_random_permutation(void){
    int n = 25;
    list<int> perm = random_permutation(n);
    //Testing if the size of the list is the same as the argument we passed
    TEST_CHECK(perm.size() == static_cast<std::list<int>::size_type>(n));

    n = 2000;
    list<int> perm1 = random_permutation(n);
    list<int> perm2 = random_permutation(n);
    list<int> perm3 = random_permutation(n);
    list<int> perm4 = random_permutation(n);

    //Testing that the lists are not the same
    TEST_CHECK(perm1 != perm2);
    TEST_CHECK(perm1 != perm3);
    TEST_CHECK(perm1 != perm4);
    TEST_CHECK(perm2 != perm3);
    TEST_CHECK(perm2 != perm4);
    TEST_CHECK(perm3 != perm4);
}

void test_get_element_at_index(void){
    Dataset double_list = {{1.0}, {2.0}, {3.0}, {4.0}, {5.0}, {6.0}};
    Data result = getElementAtIndex(double_list, 0);  
    //Testing that the func returns the values that we expect
    TEST_CHECK(result == std::vector<double>{1.0});  
    result = getElementAtIndex(double_list, 1);
    TEST_CHECK(result == std::vector<double>{2.0});  
    result = getElementAtIndex(double_list, 5);
    TEST_CHECK(result == std::vector<double>{6.0});  
}

void test_change_element_at_index(void){
    Dataset double_list = {{1.0}, {2.0}, {3.0}, {4.0}, {5.0}, {6.0}};
    changeElementAtIndex(double_list,0,{10.0});
    Data value = getElementAtIndex(double_list,0);
    //Testing if the element of the list successfully changed
    TEST_CHECK(value == std::vector<double>{10.0});
}

//Test for greedy search
void test_greedy_search(void) {
    Data d1 = {0.0, 0.0};
    Data d2 = {1.0, 1.0};
    Data d3 = {2.0, 2.0};
    Data d4 = {3.0, 3.0};

    Graph_Node G1 = create_graph_node(d1);
    Graph_Node G2 = create_graph_node(d2);
    Graph_Node G3 = create_graph_node(d3);
    Graph_Node G4 = create_graph_node(d4);

    G1->out_neighbours.push_back(G2);
    G2->out_neighbours.push_back(G1);
    G3->out_neighbours.push_back(G4);
    G4->out_neighbours.push_back(G3);

    G1->data = d1;
    G2->data = d2;
    G3->data = d3;
    G4->data = d4;

    Graph G ;
    add_node_to_graph(G,G1);
    add_node_to_graph(G,G2);
    add_node_to_graph(G,G3);
    add_node_to_graph(G,G4);

    Data query = {1.0, 2.0};

    int k = 3;
    int L = 4;

    auto result = greedy_search(G.front(), query, k, L);

    TEST_CHECK(result.first.size() == 2); 
    TEST_CHECK(result.second.size() > 0);

    auto it = result.first.begin();
    TEST_CHECK((*it)->data == G.front()->data);

    ++it;
    TEST_CHECK((*it)->data == G2->data);

    TEST_CHECK(find_node_in_graph(result.second, G1->data));  
    TEST_CHECK(find_node_in_graph(result.second, G2->data));  
}

TEST_LIST = {
    {"test_create_graph_node", test_create_graph_node },
    {"test_knn", test_knn },
    {"test_euclidean_distance", test_euclidean_distance},
    {"test_shuffle_list", test_shuffle_list},
    {"test_random_permutation", test_random_permutation},
    {"test_get_element_at_index",test_get_element_at_index},
    {"test_change_element_at_index",test_change_element_at_index},
    {"test_greedy_search", test_greedy_search},
    { 0 }
};