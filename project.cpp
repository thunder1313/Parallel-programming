#include <iostream>
#include <omp.h>
#include <unistd.h>
#include <vector>
#include <random>
#include <chrono>
#include <stdexcept>

#define THREAD_NUM 4
#define NUM_ROWS 1000
#define NUM_COLS 800

using namespace std;

// Function to generate a random matrix of size rows x cols
vector<vector<double>> generateRandomMatrix(int rows, int cols) {
    // Initialize random number generator
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 10); // Adjust range as needed

    // Create matrix
    vector<vector<double>> matrix(rows, vector<double>(cols));

    // Fill matrix with random numbers
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = dis(gen);
        }
    }

    return matrix;
}

void printMatrix(const vector<vector<double>>& matrix) {
    for (const auto& row : matrix) {
        for (double val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
    cout << endl;
}

vector<vector<double>> multiply_seq(const vector<vector<double> >& matrix1, const vector<vector<double> >& matrix2) {
    int rows1 = matrix1.size();
    int cols1 = matrix1[0].size();
    int rows2 = matrix2.size();
    int cols2 = matrix2[0].size();

    // Initialize the result matrix with zeros
    vector<vector<double>> result(rows1, vector<double>(cols2, 0));

    // Perform matrix multiplication
    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            for (int k = 0; k < cols1; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return result;
}

vector<vector<double>> multiply_omp(const vector<vector<double>>& matrix1, const vector<vector<double>>& matrix2) {
    int rows1 = matrix1.size();
    int cols1 = matrix1[0].size();
    int rows2 = matrix2.size();
    int cols2 = matrix2[0].size();

    // Initialize the result matrix with zeros
    vector<vector<double>> result(rows1, vector<double>(cols2, 0));

    // Perform matrix multiplication
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            for (int k = 0; k < cols1; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return result;
}

int main() {
    vector<vector<double>> randomMatrix1 = generateRandomMatrix(NUM_ROWS, NUM_COLS);
    vector<vector<double>> randomMatrix2 = generateRandomMatrix(NUM_COLS, NUM_ROWS);
    
    // Measure time for sequential addition
    auto start_seq = chrono::high_resolution_clock::now();
    vector<vector<double>> result_seq = multiply_seq(randomMatrix1, randomMatrix2);
    auto end_seq = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_seq = end_seq - start_seq;
    cout << "[SEQ] Time: " << elapsed_seq.count() << " seconds\n";

    // Measure time for parallel addition
    auto start_par = chrono::high_resolution_clock::now();
    vector<vector<double>> result_omp = multiply_omp(randomMatrix1, randomMatrix2);
    auto end_par = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_par = end_par - start_par;
    cout << "[OMP] Time: " << elapsed_par.count() << " seconds\n";

    return 0;
}