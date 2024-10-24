#include "../include/header.h"

int main() {
    string n = "data/bigann_query.bvecs";
    char* name = &n[0];
    vector<vector<uint8_t>> res ;
    res = bvecs_read(name);

    for (size_t i = 0; i < res.size(); i++) {
        printf("Vector %lu: ", i);
        for (size_t j = 0; j < res[i].size(); j++) {
            printf("%d ", res[i][j]);
        }
        printf("\n");
    }

    // string n = "data/siftsmall_base.fvecs";
    // char* name = &n[0];
    // vector<vector<float>> res ;
    // res = fvecs_read(name);

    // for (size_t i = 0; i < res.size(); i++) {
    //     printf("Vector %lu: ", i);
    //     for (size_t j = 0; j < res[i].size(); j++) {
    //         printf("%f ", res[i][j]);
    //     }
    //     printf("\n");
    // }
    return 0;
}