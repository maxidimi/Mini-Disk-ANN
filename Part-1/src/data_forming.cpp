#include "../include/header.h"

vector<vector<int>> fvecs_read(char* file_name){
    auto *file = fopen(file_name, "r");
    if (file == nullptr) {
        cout << "Error ";
        return {};
    }
    int num;
    cout << "Here1 ";
    fread(&num, sizeof(int), 1, file);
    int vecsizeof = 1 * 4 + num * 4;
    
    fseek(file, 0, SEEK_END);
    int a = 1;
    int bmax = ftell(file) / vecsizeof ;
    int b = bmax;
    cout<<vecsizeof/4<< "  " <<bmax<<endl;
    vector<vector<int>> arr;
    fseek(file,0, SEEK_SET);
    for (int i = 0; i < bmax; i++) {
        fseek(file, sizeof(int), SEEK_CUR);
        vector<int> v(num);
        fread(v.data(), sizeof(int), num-1, file);
        arr.push_back(v);
    }
    fclose(file);
    return arr;
}