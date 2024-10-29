#include "../include/header.h"

// Reading vectors from files of .bvecs format
Dataset bvecs_read(string file_name_s){
    char* file_name = &file_name_s[0];
    auto *file = fopen(file_name, "r");
    if (file == nullptr) {
        cout << "Error, can not open the file";
        return {};
    }

    //Reading the size of the vectors
    int num;
    size_t r = fread(&num, sizeof(int), (size_t)1, file);
    if (r != (size_t)1) {
        cout << "Error, can not read the file";
        return {};
    }
    
    int vecsizeof = 4 + num;
    
    fseek(file, 0, SEEK_END);

    // Calculate the number of vectors
    long veccount = ftell(file) / vecsizeof;
    
    Dataset arr;
    fseek(file, 0, SEEK_SET);

    //We have a loop to read every vector
    for (int i = 0; i < veccount; i++) {
        fseek(file, sizeof(int), SEEK_CUR);
        vector<uint8_t> v(num);
        r = fread(v.data(), sizeof(uint8_t), (size_t)num, file);
        vector<data_t> v2(v.begin(), v.end());
        if (r != (size_t)num) {
            cout << "Error, can not read the file";
            return {};
        }
        arr.push_back(v2);
    }
    fclose(file);

    return arr;
}

// Reading vectors from files of .fvecs format
Dataset fvecs_read(string file_name_s){
    char* file_name = &file_name_s[0];
    auto *file = fopen(file_name, "r");
    if (file == nullptr) {
        cout << "Error, can not open the file";
        return {};
    }

    //Reading the size of the vectors
    int num;
    size_t r = fread(&num, sizeof(int), (size_t)1, file);
    if (r != (size_t)1) {
        cout << "Error, can not read the file";
        return {};
    }
    cout<<"here"<<endl;
    int vecsizeof = 4 + num*4;
    
    fseek(file, 0, SEEK_END);

    // Calculate the number of vectors
    long veccount = ftell(file) / vecsizeof;
    Dataset arr;
    fseek(file, 0, SEEK_SET);

    //We have a loop to read every vector
    for (int i = 0; i < veccount; i++) {
        fseek(file, sizeof(int), SEEK_CUR);
        vector<float> v(num);
        r = fread(v.data(), sizeof(float), (size_t)num, file);
        vector<data_t> v2(v.begin(), v.end());

        if (r != (size_t)num) {
            cout << "Error, can not read the file";
            return {};
        }
        
        arr.push_back(v2);
    }
    fclose(file);

    return arr;
}

// Reading vectors from files of .ivecs format
Dataset ivecs_read(string Filename_s) {
    char* Filename = &Filename_s[0];
    auto fid = fopen(Filename,"r");

    if (fid == NULL) {
        cout << "I/O error : Unable to open the file "<< Filename << endl;
        return {};
    }

    int num;
    size_t r = fread(&num, sizeof(int), (size_t)1, fid);
    if (r != (size_t)1) {
        cout << "Error, can not read the file";
        return {};
    }

    int vecsizeof = 4 + num * 4;

    fseek(fid , 0, SEEK_END);

    long vecnum = ftell(fid) / vecsizeof;
    Dataset arr;

    fseek(fid, 0, SEEK_SET);

    for(int i = 0; i < vecnum; i++) {
        fseek(fid, sizeof(int), SEEK_CUR);
        vector<int> v(num);
        r = fread(v.data(), sizeof(int), (size_t)num, file);
        vector<data_t> v2(v.begin(), v.end());
        if (r != (size_t)(num - 1)) {
            cout << "Error, can not read the file";
            return {};
        }
        arr.push_back(v2);
    }
    fclose(fid);

    return arr;
}