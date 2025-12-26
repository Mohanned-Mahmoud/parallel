#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <omp.h>

const int SIZE = 50000000; // Increased to 50M to stress memory bandwidth

void process_array(const std::vector<double>& A, std::vector<double>& B) {
    #pragma omp parallel for
    for (int i = 0; i < SIZE; ++i) {
        B[i] = std::sin(A[i]);
    }
}

int main() {
    std::vector<double> A(SIZE, 1.5);
    std::vector<double> B(SIZE);

    auto start = std::chrono::high_resolution_clock::now();
    process_array(A, B);
    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

    std::cout << duration << std::endl; 
    return 0;
}