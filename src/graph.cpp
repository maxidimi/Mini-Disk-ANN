#include "../include/header.h"

// Creates a graph node
Graph_Node create_graph_node(const Data &data) {
    Graph_Node node = new graph_node;
    node->data = data;
    
    return node;
}

// Adds a node to the graph
void add_node_to_graph(Graph &graph, Graph_Node &node) {
    graph.push_back(node);
    node->indx = graph.size() - 1;
}

// Adds an edge to the graph
void add_edge_to_graph(Graph_Node &from, int to) {
    if (from->indx != to) // Avoid self-loops
        from->out_neighbours.insert(to);
}

// Finds a node in the graph
Graph_Node find_data_in_graph(Graph &graph, const Data &data) {
    auto it = find_if(graph.begin(), graph.end(), [&data](Graph_Node node) {
        return node->data == data;
    });
    if (it != graph.end()) {
        return *it;
    }
    return nullptr;
}

// Storing the graph in a binary file
void store_graph(const Graph &G, string filename) {
    
    // Open the file
    ofstream file(filename, ios::binary);
    if (!file) return;

    // Write the number of nodes
    int num_nodes = (int)G.size();
    file.write(reinterpret_cast<char*>(&num_nodes), sizeof(int));

    // Write the data and the out neighbours of each node
    for (const auto &node : G) {
        // Write the data
        int dim = (int)node->data.size();
        file.write(reinterpret_cast<char*>(&dim), sizeof(int));
        file.write(reinterpret_cast<char*>(node->data.data()), dim * sizeof(data_t));

        // Write the number of out neighbours
        int num_neighbours = (int)node->out_neighbours.size();
        file.write(reinterpret_cast<char*>(&num_neighbours), sizeof(int));

        // Write the out neighbours indices
        for (auto neighbour : node->out_neighbours) {
            file.write(reinterpret_cast<char*>(&neighbour), sizeof(int));
        }
    }

    file.close();

    return;
}

// Reading the graph from a binary file
Graph read_graph(string filename) {
    Graph G;
    ifstream file(filename, ios::binary);
    if (!file) {
        return G;
    }

    // Read the number of nodes
    int num_nodes;
    file.read(reinterpret_cast<char*>(&num_nodes), sizeof(int));

    // Read the data and the out neighbours of each node
    for (int i = 0; i < num_nodes; i++) {
        // Read the data
        int dim;
        file.read(reinterpret_cast<char*>(&dim), sizeof(int));
        Data data(dim);
        file.read(reinterpret_cast<char*>(data.data()), dim * sizeof(data_t));

        // Create the node
        Graph_Node node = create_graph_node(data);

        // Read the index of the node
        node->indx = i;

        // Read the number of out neighbours
        int num_neighbours;
        file.read(reinterpret_cast<char*>(&num_neighbours), sizeof(int));

        // Read the out neighbours indices
        for (int j = 0; j < num_neighbours; j++) {
            int neighbour;
            file.read(reinterpret_cast<char*>(&neighbour), sizeof(int));
            node->out_neighbours.insert(neighbour);
        }

        // Add the node to the graph
        add_node_to_graph(G, node);
    }

    file.close();

    return G;
}
