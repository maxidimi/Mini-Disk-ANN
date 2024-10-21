#include "../include/header.h"

vector<vector<float>> fvecs_read(const string& filename, vector<int> bounds = {}) {
    ifstream file(filename, binary);
    if (!file.is_open()) {
        throw runtime_error("I/O error: Unable to open the file " + filename);
    }

    // Read the vector size (first integer)
    int d;
    file.read((char*)(&d), sizeof(int));

    size_t vecsizeof = sizeof(int) + d * sizeof(float);

    // Get the number of vectors by seeking to the end of the file
    file.seekg(0, end);
    size_t file_size = file.tellg();
    size_t bmax = file_size / vecsizeof;

    // Set bounds: a (starting index), b (ending index)
    int a = 1;
    int b = bmax;
    if (!bounds.empty()) {
        if (bounds.size() == 1) {
            b = bounds[0];
        } else if (bounds.size() == 2) {
            a = bounds[0];
            b = bounds[1];
        }
    }

    assert(a >= 1);
    if (b > bmax) {
        b = bmax;
    }

    if (b == 0 || b < a) {
        return {};  // Return empty vector if b is invalid
    }

    // Compute number of vectors to read
    int n = b - a + 1;

    // Go to the starting position of the first vector
    file.seekg((a - 1) * vecsizeof, beg);

    // Read n vectors
    vector<float> buffer((d + 1) * n);
    file.read((char*)(buffer.data()), buffer.size() * sizeof(float));

    // Reshape buffer into matrix of size (d+1) x n
    vector<vector<float>> v(d + 1, vector<float>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < d + 1; ++j) {
            v[j][i] = buffer[i * (d + 1) + j];
        }
    }

    // Check if the first column (dimension of the vectors) is correct
    for (int i = 1; i < n; ++i) {
        assert(v[0][i] == v[0][0]);
    }

    // Remove the first row (dimensions) to get only the vectors
    vector<vector<float>> vectors(d, vector<float>(n));
    for (int i = 0; i < d; ++i) {
        for (int j = 0; j < n; ++j) {
            vectors[i][j] = v[i + 1][j];
        }
    }

    file.close();
    return vectors;
}

vector<vector<int>> ivecs_read(const string& filename, vector<int> bounds = {}) {
    ifstream file(filename, binary);
    if (!file.is_open()) {
        throw runtime_error("I/O error: Unable to open the file " + filename);
    }

    // Read the vector size (first integer)
    int d;
    file.read(reinterpret_cast<char*>(&d), sizeof(int));

    size_t vecsizeof = sizeof(int) + d * sizeof(int);

    // Get the number of vectors by seeking to the end of the file
    file.seekg(0, end);
    size_t file_size = file.tellg();
    size_t bmax = file_size / vecsizeof;

    // Set bounds: a (starting index), b (ending index)
    int a = 1;
    int b = bmax;
    if (!bounds.empty()) {
        if (bounds.size() == 1) {
            b = bounds[0];
        } else if (bounds.size() == 2) {
            a = bounds[0];
            b = bounds[1];
        }
    }

    assert(a >= 1);
    if (b > bmax) {
        b = bmax;
    }

    if (b == 0 || b < a) {
        return {};  // Return empty vector if b is invalid
    }

    // Compute the number of vectors to read
    int n = b - a + 1;

    // Go to the starting position of the first vector
    file.seekg((a - 1) * vecsizeof, beg);

    // Read n vectors
    vector<int> buffer((d + 1) * n);
    file.read(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(int));

    // Reshape buffer into matrix of size (d+1) x n
    vector<vector<int>> v(d + 1, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < d + 1; ++j) {
            v[j][i] = buffer[i * (d + 1) + j];
        }
    }

    // Check if the first column (dimension of the vectors) is correct
    for (int i = 1; i < n; ++i) {
        assert(v[0][i] == v[0][0]);
    }

    // Remove the first row (dimensions) to get only the vectors
    vector<vector<int>> vectors(d, vector<int>(n));
    for (int i = 0; i < d; ++i) {
        for (int j = 0; j < n; ++j) {
            vectors[i][j] = v[i + 1][j];
        }
    }

    file.close();
    return vectors;
}

vector<vector<uint8_t>> bvecs_read(const string& filename, vector<int> bounds = {}) {
    ifstream file(filename, binary);
    if (!file.is_open()) {
        throw runtime_error("I/O error: Unable to open the file " + filename);
    }

    // Read the vector size (first integer)
    int d;
    file.read(reinterpret_cast<char*>(&d), sizeof(int));

    size_t vecsizeof = sizeof(int) + d * sizeof(uint8_t);

    // Get the number of vectors by seeking to the end of the file
    file.seekg(0, end);
    size_t file_size = file.tellg();
    size_t bmax = file_size / vecsizeof;

    // Set bounds: a (starting index), b (ending index)
    int a = 1;
    int b = bmax;
    if (!bounds.empty()) {
        if (bounds.size() == 1) {
            b = bounds[0];
        } else if (bounds.size() == 2) {
            a = bounds[0];
            b = bounds[1];
        }
    }

    assert(a >= 1);
    if (b > bmax) {
        b = bmax;
    }

    if (b == 0 || b < a) {
        return {};  // Return empty vector if b is invalid
    }

    // Compute the number of vectors to read
    int n = b - a + 1;

    // Go to the starting position of the first vector
    file.seekg((a - 1) * vecsizeof, beg);

    // Read n vectors
    vector<uint8_t> buffer((d + 4) * n);
    file.read(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(uint8_t));

    // Reshape buffer into matrix of size (d+4) x n
    vector<vector<uint8_t>> v(d + 4, vector<uint8_t>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < d + 4; ++j) {
            v[j][i] = buffer[i * (d + 4) + j];
        }
    }

    // Check if the first four columns (dimensions) are consistent across vectors
    for (int i = 1; i < n; ++i) {
        assert(v[0][i] == v[0][0]);
        assert(v[1][i] == v[1][0]);
        assert(v[2][i] == v[2][0]);
        assert(v[3][i] == v[3][0]);
    }

    // Remove the first 4 rows (dimension-related) to get only the vectors
    vector<vector<uint8_t>> vectors(d, vector<uint8_t>(n));
    for (int i = 0; i < d; ++i) {
        for (int j = 0; j < n; ++j) {
            vectors[i][j] = v[i + 4][j];
        }
    }

    file.close();
    return vectors;
}
