#include "../include/header.h"

int main(int argc, char *argv[]) {
    // Parameters for the Vamana indexing
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " n dim k\n";
        return 1;
    }
    int n = atoi(argv[1]); int dim = atoi(argv[2]); int k = atoi(argv[3]);
    if (n <= 1 || dim <= 0 || k <= 1 || k >= n) {
        cout << "Invalid parameters\n";
        return 1;
    }
    srand((unsigned int)time(0));
    double a = 1.2; int R = 5; int L = 89;
    /*int L = k + rand()/(RAND_MAX/100);
    if ((int)(log2(n) - 1) == 0) { 
        R = 1;
    } else {
        R = rand() % (int)(log2(n) - 1) + 1;
    }
    if (R < 1) {
        R = 1;
    }*/
    cout << "k: " << k << " || R: " << R << " || L: " << L << "|| a: " << a << " || n: " << n << " || dim: " << dim << endl;

    // Create a dataset of points
    Dataset dataset = random_dataset(n, dim);

    // Query point to search nearest neighbors for
    Data query = random_query(dim);

    // Start the timer
    clock_t start = clock();

    // Create the Vamana index
    Graph G = vamana_indexing(dataset, a, L, R);

    // Perform greedy search starting from the first node
    auto result_p = greedy_search(G.front(), query, k, L);
    auto result = result_p.first; auto visited = result_p.second;

    // End the timer
    clock_t end = clock();
    double elapsed_time = double(end - start) / CLOCKS_PER_SEC;
    cout << "Time taken: " << elapsed_time << " seconds\n";
    
    // Calculate the Euclidean distances of each point from the query
    vector<pair<Data, double>> distances;
    for (const auto &data : dataset) {
        double dist = euclidean_distance(data, query);
        distances.emplace_back(data, dist);
    }
    
    // Sort distances in ascending order
    sort(distances.begin(), distances.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });
    
    // Get the expected nearest neighbors
    vector<Data> expected_neighbors;
    for (size_t i = 0; i < (size_t)k; i++) {
        expected_neighbors.push_back(distances[i].first);
    }
    
    // Print the distances with corresponding points
    /*cout << "\n\nDistances from query point (" << query[0] << ", " << query[1] << "):\n";
    for (const auto &[point, distance] : distances) {
        cout << "Point: (" << point[0] << ", " << point[1] << ") - Distance: " << distance << endl;
    }
    cout << "\nExpected nearest neighbors:\n";
    for (const auto &neighbor : expected_neighbors) {
        cout << "\tPoint: (" << neighbor[0] << ", " << neighbor[1] << ")\n";
    }
    cout << "Greedy search results:\n";
    for (const auto &node : result) {
        if (node == nullptr) {
            cout << "\tPoint: (nullptr)\n";
        } else 
        {cout << "\tPoint: (" << node->data[0] << ", " << node->data[1] << ")\n";}
    }*/

    // Check if the number of neighbors found matches k
    if (result.size() == (size_t)k) cout << "Number of neighbors found matches k\n";
    else {cout << "Number of neighbors found does not match k. Found: " << result.size() << " Expected: " << k << endl;}
    int foundC = 0;
    
    // Check if the neighbor is in the expected neighbors
    for (const auto &node : result) {
        if (node == nullptr) {
            cout << "Error: nullptr\n";
            continue;
        }
        bool found = false;
        for (const auto &expected : expected_neighbors) {
            if (node->data == expected) {
                found = true;
                break;
            }
        }
        if (!found) {cout << "Error: Neighbor not found in expected neighbors\n";}
        else {foundC++;}
    }
    cout << "Number of neighbors found in expected neighbors: " << foundC << endl;
    
    return 0;
}
