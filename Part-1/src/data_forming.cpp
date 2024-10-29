#include "../include/header.h"

//Func for reading vectors from a file
vector<vector<uint8_t>> bvecs_read(char* file_name){
    auto *file = fopen(file_name, "r");
    if (file == nullptr) {
        cout << "Error, can not open the file";
        return {};
    }
    //Reading the size of the vectors
    int num;
    fread(&num, sizeof(int), 1, file);
    
    int vecsizeof = 4 + num;
    
    fseek(file, 0, SEEK_END);

    // Calculate the number of vectors
    long veccount = ftell(file) / vecsizeof ;
    
    vector<vector<uint8_t>> arr;
    fseek(file,0, SEEK_SET);
    //We have a loop to read every vector
    for (int i = 0; i < veccount; i++) {
        fseek(file, sizeof(int), SEEK_CUR);
        vector<uint8_t> v(num);
        fread(v.data(), sizeof(uint8_t), num, file);
        arr.push_back(v);
    }
    fclose(file);
    return arr;
}


vector<vector<float>> fvecs_read(char* file_name){
    auto *file = fopen(file_name, "r");
    if (file == nullptr) {
        cout << "Error, can not open the file";
        return {};
    }
    //Reading the size of the vectors
    int num;
    fread(&num, sizeof(int), 1, file);
    
    int vecsizeof = 4 + num*4;
    
    fseek(file, 0, SEEK_END);
    // Calculate the number of vectors
    long veccount = ftell(file) / vecsizeof ;

    vector<vector<float>> arr;
    fseek(file,0, SEEK_SET);
    //We have a loop to read every vector
    for (int i = 0; i < veccount; i++) {
        fseek(file, sizeof(int), SEEK_CUR);
        vector<float> v(num);
        fread(v.data(), sizeof(int), num, file);
        arr.push_back(v);
    }
    fclose(file);
    return arr;
}


vector<vector<int>> ivecs_read(char * file_name) {
    auto *file = fopen(file_name, "r");
    if (file == nullptr) {
        cout << "Error, can not open the file";
        return {};
    }
    //Reading the size of the vectors
    int num;
    fread(&num, sizeof(int), 1, file);
    
    int vecsizeof = 4 + num*4;
    
    fseek(file, 0, SEEK_END);
    // Calculate the number of vectors
    long veccount = ftell(file) / vecsizeof ;
    vector<vector<int>> arr;
    fseek(file,0, SEEK_SET);
    //We have a loop to read every vector
    for (int i = 0; i < veccount; i++) {
        fseek(file, sizeof(int), SEEK_CUR);
        vector<int> v(num);
        fread(v.data(), sizeof(int), num, file);
        arr.push_back(v);
    }
    fclose(file);
    return arr;
}