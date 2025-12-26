#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <cstdlib>

// N=1000 provides a better workload for visibility than 512
const int N = 1000; 

void matrix_mult(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B,
                 std::vector<std::vector<double>>& C) {
    // Collapse(2) parallelizes both outer loops for better distribution
    #pragma omp parallel for collapse(2)
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
    std::vector<std::vector<double>> A(N, std::vector<double>(N, 1.0));
    std::vector<std::vector<double>> B(N, std::vector<double>(N, 2.0));
    std::vector<std::vector<double>> C(N, std::vector<double>(N));

    auto start = std::chrono::high_resolution_clock::now();
    matrix_mult(A, B, C);
    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

    std::cout << duration << std::endl; // Output ONLY the number for easy parsing
    return 0;
}