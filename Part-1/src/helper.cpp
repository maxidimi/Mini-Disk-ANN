#include "../include/header.h"

int euclidean_distance(Data d1, Data d2) {
    auto distance = 0;
    for (uint i = 0; i < d1.size(); i++) {
        distance += (d1[i] - d2[i]) * (d1[i] - d2[i]);
    }
    
    distance = sqrt(distance);

    return (int)distance;
}

void random_permutation(vector<int> &sigma, Dataset P) {
    int n = P.size();

    for (int i = 0; i < n; i++) 
        sigma.push_back(i);
    
    random_shuffle(sigma.begin(), sigma.end());
}
