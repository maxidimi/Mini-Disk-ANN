#include "../include/header.h"
#include "../include/acutest.h"

void test_create_graph_node(void) {
    Data data = {1, 2, 3};

    Graph_Node node = create_graph_node(data);

    TEST_CHECK(node->data == data);

    TEST_CHECK(node->out_neighbours.size() == 0);
}

void test_add_node_to_graph(void) {
    Data data = {1, 2, 3};

    Graph_Node node = create_graph_node(data);

    Graph graph = {node};

    Data data1 = {4, 5, 6};

    Graph_Node node1 = create_graph_node(data1);

    add_node_to_graph(graph, node1);

    TEST_CHECK(graph.size() == 2);
    TEST_CHECK(graph[1] == node1);
}

void test_add_edge_to_graph(void) {
    Data data = {1, 2, 3};

    Graph_Node node = create_graph_node(data);

    Data data1 = {4, 5, 6};

    Graph_Node node1 = create_graph_node(data1);

    add_edge_to_graph(node, node1);

    TEST_CHECK(node->out_neighbours.size() == 1);
    TEST_CHECK(node->out_neighbours.find(node1) != node->out_neighbours.end());

    add_edge_to_graph(node, node1);
    TEST_CHECK(node->out_neighbours.size() == 1);
}

void test_find_node_in_graph(void){
    Data data = {1, 2, 3};
    Graph_Node node = create_graph_node(data);
    Graph graph = {node};

    // Test for a data that is in the graph
    Graph_Node found = find_node_in_graph(graph, data);
    TEST_CHECK(found == node);

    // Test for changing the data of the node
    Data new_data = {4, 5, 6};
    found->data = new_data;
    Graph_Node found_changed = find_node_in_graph(graph, new_data);
    TEST_CHECK(found_changed->data == new_data);

    // Test for a data that is not in the graph
    Graph_Node snot_found = find_node_in_graph(graph, data);
    TEST_CHECK(snot_found == nullptr);
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
    vector<int> perm = random_permutation(n);

    //Testing if the size of the list is the same as the argument we passed
    TEST_CHECK(perm.size() == static_cast<list<int>::size_type>(n));

    n = 2000;
    vector<int> perm1 = random_permutation(n);
    vector<int> perm2 = random_permutation(n);
    vector<int> perm3 = random_permutation(n);
    vector<int> perm4 = random_permutation(n);

    //Testing that the lists are not the same
    TEST_CHECK(perm1 != perm2);
    TEST_CHECK(perm1 != perm3);
    TEST_CHECK(perm1 != perm4);
    TEST_CHECK(perm2 != perm3);
    TEST_CHECK(perm2 != perm4);
    TEST_CHECK(perm3 != perm4);
}

void test_medoid(void) {
    Dataset P = {{0, 0}, {0, 4}, {2, 3}, {4, 4}, {4, 0}};
    
    int s = medoid(P);

    int expected = 2;

    TEST_CHECK(s == expected);
}

void test_greedy_search(void) {
    // Define the parameters for the test
    srand((unsigned int)time(0));
    int n = 500; int dim = 2;

    // Create a dataset of points
    Dataset dataset = random_dataset(n, dim);

    // Query point to search nearest neighbors for
    Data query = random_query(dim);

    // Parameters for the Vamana indexing
    int k = 8;
    int L = k + rand()/(RAND_MAX/100);
    int R = log2(n) + rand()/(RAND_MAX/100);
    double a = 2.0;

    // Create the Vamana index
    Graph G = vamana_indexing(dataset, a, L, R);

    // Perform greedy search starting from the first node
    auto result_p = greedy_search(G.front(), query, k, L);
    auto result = result_p.first; auto visited = result_p.second;
    
    // Calculate the Euclidean distances of each point from the query
    vector<pair<Data, data_t>> distances;
    for (const auto& data : dataset) {
        long double dist = euclidean_distance(data, query);
        distances.emplace_back(data, dist);
    }

    // Sort distances in ascending order
    sort(distances.begin(), distances.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });
    
    // Get the expected nearest neighbors
    vector<Data> expected_neighbors;
    for (size_t i = 0; i < static_cast<size_t>(k); i++) {
        expected_neighbors.push_back(distances[i].first);
    }

    // Check if the number of neighbors found matches k
    TEST_CHECK(result.size() == static_cast<size_t>(k));
    
    // Check if the neighbor is in the expected neighbors
    for (const auto& node : result) {
        if (node == nullptr) {
            TEST_CHECK(false);
            continue;
        }
        bool found = false;
        for (const auto& expected : expected_neighbors) {
            if (node->data == expected) {
                found = true;
                break;
            }
        }
        TEST_CHECK(found);
    }
}


TEST_LIST = {
    {"test_create_graph_node", test_create_graph_node },
    {"test_add_node_to_graph", test_add_node_to_graph},
    {"test_add_edge_to_graph", test_add_edge_to_graph},
    {"test_find_node_in_graph", test_find_node_in_graph},
    {"test_euclidean_distance", test_euclidean_distance},
    {"test_random_permutation", test_random_permutation},
    {"test_medoid", test_medoid},
    {"test_greedy_search", test_greedy_search},
    { 0 }
};
