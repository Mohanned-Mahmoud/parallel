// array_processing.cpp: Sequential Array Processing (Memory Bound)
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <cstdlib>

// 50 Million elements to ensure the CPU has to wait for memory
const int SIZE = 50000000; 

void initialize(std::vector<double>& A) {
    for (int i = 0; i < SIZE; ++i) {
        A[i] = 1.5; 
    }
}

// Sequential Processing: Apply sin() to each element
void process_array(const std::vector<double>& A, std::vector<double>& B) {
    for (int i = 0; i < SIZE; ++i) {
        B[i] = std::sin(A[i]); 
    }
}

int main() {
    std::vector<double> A(SIZE);
    std::vector<double> B(SIZE);

    initialize(A);

    // Timing Execution
    auto start = std::chrono::high_resolution_clock::now();
    process_array(A, B);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> duration = end - start;
    std::cout << duration.count() << std::endl;
    
    return 0;
}