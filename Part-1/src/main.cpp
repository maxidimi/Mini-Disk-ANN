#include "../include/header.h"

int main() {
    // Create some data points
    Data d1 = {0.0, 0.0};
    Data d2 = {1.0, 1.0};
    Data d3 = {2.0, 2.0};
    Data d4 = {3.0, 3.0};

    Graph_Node G1 = create_graph_node(d1);
    Graph_Node G2 = create_graph_node(d2);
    Graph_Node G3 = create_graph_node(d3);
    Graph_Node G4 = create_graph_node(d4);


    // Define neighbors
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


    // Query point
    Data query = {1.5, 1.5};


    // k = 2, L = 3 (for example)
    int k = 2;
    int L = 3;

    // Call the greedy search function
    auto result = greedy_search(G.front() , query , k, L);

    // Print the results
    std::cout << "Searching List (Approx. k nearest points):" << std::endl;
    for (const auto& node : result.first) {
        std::cout << "(" << node->data[0] << ", " << node->data[1] << ")" << std::endl;
    }

    std::cout << "\nVisited List:" << std::endl;
    for (const auto& node : result.second) {
        std::cout << "(" << node->data[0] << ", " << node->data[1] << ")" << std::endl;
    }

    return 0;
}