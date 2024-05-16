#include <iostream>
#include <omp.h>
#include <unistd.h>
#include <vector>
#include <random>
#include <chrono>

#define THREAD_NUM 6
#define NUM_COLS 20000
#define NUM_ROWS 20000

using namespace std;

// Function to generate a random matrix of size rows x cols
vector<vector<int> > generateRandomMatrix(int rows, int cols) {
    // Initialize random number generator
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100); // Adjust range as needed

    // Create matrix
    vector<vector<int> > matrix(rows, vector<int>(cols));

    // Fill matrix with random numbers
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = dis(gen);
        }
    }

    return matrix;
}

// Function to add two matrices sequentially
vector<vector<int> > addMatricesSequential(const vector<vector<int> > &a, const vector<vector<int> > &b) {
    int rows = a.size();
    int cols = a[0].size();
    vector<vector<int> > result(rows, vector<int>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }

    return result;
}

// Function to add two matrices in parallel using OpenMP
vector<vector<int> > addMatricesParallel(const vector<vector<int> > &a, const vector<vector<int> > &b) {
    int rows = a.size();
    int cols = a[0].size();
    vector<vector<int> > result(rows, vector<int>(cols));

    #pragma omp parallel for num_threads(THREAD_NUM) collapse(2)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }

    return result;
}

int main() {
    vector<vector<int> > randomMatrix1 = generateRandomMatrix(NUM_ROWS, NUM_COLS);
    vector<vector<int> > randomMatrix2 = generateRandomMatrix(NUM_ROWS, NUM_COLS);
    
    // Measure time for sequential addition
    auto start_seq = chrono::high_resolution_clock::now();
    vector<vector<int> > resultSeq = addMatricesSequential(randomMatrix1, randomMatrix2);
    auto end_seq = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_seq = end_seq - start_seq;
    cout << "Time taken for sequential addition: " << elapsed_seq.count() << " seconds\n";

    // Measure time for parallel addition
    auto start_par = chrono::high_resolution_clock::now();
    vector<vector<int> > resultPar = addMatricesParallel(randomMatrix1, randomMatrix2);
    auto end_par = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_par = end_par - start_par;
    cout << "Time taken for parallel addition: " << elapsed_par.count() << " seconds\n";

    // Print matrix
    // for (const auto& row : randomMatrix1) {
    //     for (int val : row) {
    //         cout << val << " ";
    //     }
    //     cout << endl;
    // }

    // cout << endl << endl;

    // for (const auto& row : randomMatrix2) {
    //     for (int val : row) {
    //         cout << val << " ";
    //     }
    //     cout << endl;
    // }

    return 0;
}