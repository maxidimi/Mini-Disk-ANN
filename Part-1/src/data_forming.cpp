#include "../include/header.h"

void* fvecs_read(char* file_name){
    auto *file = fopen(file_name, "r");
    if (file == nullptr) {
        cout << "Error ";
        return;
    }
    int num;
    fread(&num, sizeof(int), 1, file);
    int vecsizeof = 1 * 4 + num * 4;
    fseek(file, 0, SEEK_END);
    int a = 1;
    int bmax = ftell(file) / vecsizeof ;
    int b = bmax;
    vector<int> arr[bmax][vecsizeof];
    fseek(file,0, SEEK_SET);
    for(int i = 0; i < bmax; i++) {
        fseek(file,(long) sizeof(int) , SEEK_CUR);
        vector<int> v ;
        fread(&v, sizeof(int) , vecsizeof ,file);
        arr[i]->assign(v.begin(),v.end());
    }
}