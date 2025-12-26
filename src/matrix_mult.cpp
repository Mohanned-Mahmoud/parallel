// matrix_mult.cpp: Sequential Matrix Multiplication (Compute Bound)
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>

// N=1000 provides a substantial workload
const int N = 1000; 

// Initialize matrices with simple values
void initialize(std::vector<std::vector<double>>& A, std::vector<std::vector<double>>& B) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = 1.0; 
            B[i][j] = 2.0;
        }
    }
}

// Sequential Algorithm: C[i][j] += A[i][k] * B[k][j]
void matrix_mult(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B,
                 std::vector<std::vector<double>>& C) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double sum = 0;
            for (int k = 0; k < N; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

int main() {
    // Allocation
    std::vector<std::vector<double>> A(N, std::vector<double>(N));
    std::vector<std::vector<double>> B(N, std::vector<double>(N));
    std::vector<std::vector<double>> C(N, std::vector<double>(N));

    initialize(A, B);

    // Timing Execution
    auto start = std::chrono::high_resolution_clock::now();
    matrix_mult(A, B, C);
    auto end = std::chrono::high_resolution_clock::now();
    
    // Output only the duration (seconds) for easy CSV parsing
    std::chrono::duration<double> duration = end - start;
    std::cout << duration.count() << std::endl;
    
    return 0;
}