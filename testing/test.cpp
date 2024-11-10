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
    node->indx = 0;

    Data data1 = {4, 5, 6};
    Graph_Node node1 = create_graph_node(data1);
    node1->indx = 1;

    add_edge_to_graph(node, node1->indx);

    TEST_CHECK(node->out_neighbours.size() == 1);
    TEST_CHECK(node->out_neighbours.find(node1->indx) != node->out_neighbours.end());

    add_edge_to_graph(node, node1->indx);
    TEST_CHECK(node->out_neighbours.size() == 1);
}

void test_find_data_in_graph(void){
    Graph graph;

    Data data = {4, 5, 6};
    Graph_Node node = create_graph_node(data);
    graph.push_back(node);

    //Test for existing data
    Graph_Node result = find_data_in_graph(graph, data);
    TEST_CHECK(result == node);

    //Test for not existing data
    Data data1 = {7,8,9};
    result = find_data_in_graph(graph, data1);
    TEST_CHECK(result == nullptr);

}

void test_read_graph(void){
    //Test on a file that does not existing
    Graph G = read_graph("random_file");
    TEST_CHECK(G.empty());  
}

void test_store_graph(void){
    
    Data data1 = {1,2,3};
    Graph_Node node1 = create_graph_node(data1);
    node1->indx = 0;
    node1->out_neighbours.insert(1);

    Data data2 = {4,5,6};
    Graph_Node node2 = create_graph_node(data2);
    node2->indx = 1;
    node2->out_neighbours.insert(0);

    Graph G;
    add_node_to_graph(G, node1);
    add_node_to_graph(G, node2);

    string filename = "test_graph.dat";
    store_graph(G, filename);

    Graph G_read = read_graph(filename);
    TEST_CHECK(G_read.size() == 2);  
}

void test_euclidean_distance(void){//squared euclidean distance
    Data d1 = {20};
    Data d2 = {10};
    //Test for 1D
    TEST_CHECK(fabs(euclidean_distance(d1, d2) - 100) < 1e-6);
    
    d1 = {2, 5};
    d2 = {3, 4};
    //Test for 2D
    TEST_CHECK(fabs(euclidean_distance(d1, d2) - 2) < 1e-6);

    d1 = {3, 6, 9};
    d2 = {2, 4, 6};
    //Test for 3D
    TEST_CHECK(fabs(euclidean_distance(d1, d2) - 14) < 1e-6);
    
    d1 = {3, 6, 9, 12};
    d2 = {2, 4, 6, 8};
    //Test for 4D
    TEST_CHECK(fabs(euclidean_distance(d1, d2) - 30) < 1e-6);
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

void test_L_m_V(void){
    vector<int> L = {1,2,3,4,5,6,7,8,9};
    vector<int> m = {5,6,7,8,9};
    vector<int> LV;
    L_m_V(L,m,LV);

    //Testing if the result is the elements in L that are not in V
    vector<int> exp = {1,2,3,4};
    TEST_CHECK(exp == LV);
}

void test_greedy_search(void) {
    // Define the parameters for the test
    srand((unsigned int)time(0));
    int n = 3000; int dim = 2;

    // Create a random dataset and query
    Dataset dataset = random_dataset(n, dim);
    Data query = random_query(dim);

    // Parameters for the Vamana indexing
    int k = 100; int L = 100;
    int R = 15; double a = 1.2;

    // Create the Vamana index
    Graph G = vamana_indexing(dataset, a, L, R);

    // Perform greedy search starting from the first node
    Graph_Node s = G.front();
    auto result_p = greedy_search(G, s, query, k, L);
    auto L_result = result_p.first;    
    auto visited = result_p.second;
    
    // Calculate the Euclidean distances of each point from the query
    vector<pair<int, euclidean_t>> distances;
    for (size_t i = 0; i < dataset.size(); ++i) {
        euclidean_t dist = euclidean_distance(dataset[i], query);
        distances.emplace_back(i, dist);
    }

    // Sort distances in ascending order
    sort(distances.begin(), distances.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });
    
    // Get the expected nearest neighbors
    vector<int> expected_neighbors;
    for (size_t i = 0; i < static_cast<size_t>(k); i++) {
        expected_neighbors.push_back(distances[i].first);
    }

    // Check if the number of neighbors found matches k
    TEST_CHECK(static_cast<int>(L_result.size()) == k);
    
    // Check if the neighbor is in the expected neighbors
    for (const auto& index : L_result) {
        bool found = false;
        for (const auto& expected_index : expected_neighbors) {
            if (index == expected_index) {
                found = true;
                break;
            }
        }
    TEST_CHECK(found); // Check that index is among the expected nearest neighbors
    }
}




TEST_LIST = {
    {"test_create_graph_node", test_create_graph_node },
    {"test_add_node_to_graph", test_add_node_to_graph},
    {"test_add_edge_to_graph", test_add_edge_to_graph},
    {"test_find_data_in_graph", test_find_data_in_graph},
    {"test_read_graph", test_read_graph},
    {"test_store_graph", test_store_graph},
    {"test_euclidean_distance", test_euclidean_distance},
    {"test_random_permutation", test_random_permutation},
    {"test_medoid", test_medoid},
    {"test_L_m_V", test_L_m_V},
    {"test_greedy_search", test_greedy_search},
    { 0 }
};
