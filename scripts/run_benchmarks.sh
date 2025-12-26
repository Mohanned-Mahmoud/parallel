#!/bin/bash

# 1. Compile all files
echo "Compiling Benchmarks..."
g++ -O2 src/matrix_mult.cpp -o bin_matrix_seq
g++ -O2 -fopenmp src/matrix_mult_parallel.cpp -o bin_matrix_par

g++ -O2 src/array_processing.cpp -o bin_array_seq
g++ -O2 -fopenmp src/array_processing_parallel.cpp -o bin_array_par

g++ -O2 src/merge_sort.cpp -o bin_sort_seq
g++ -O2 -fopenmp src/merge_sort_parallel.cpp -o bin_sort_par

# Prepare CSV output
echo "Benchmark,Threads,Time" > benchmark_data.csv

# 2. Run Sequential Tests
echo "Running Sequential Baseline..."
T_MAT_SEQ=$(./bin_matrix_seq)
echo "Matrix,1,$T_MAT_SEQ" >> benchmark_data.csv

T_ARR_SEQ=$(./bin_array_seq)
echo "Array,1,$T_ARR_SEQ" >> benchmark_data.csv

T_SORT_SEQ=$(./bin_sort_seq)
echo "Sort,1,$T_SORT_SEQ" >> benchmark_data.csv

# 3. Run Parallel Tests (2, 4, 8 Threads)
for t in 2 4 8
do
    echo "Running Parallel with $t Threads..."
    export OMP_NUM_THREADS=$t
    
    T_MAT=$(./bin_matrix_par)
    echo "Matrix,$t,$T_MAT" >> benchmark_data.csv
    
    T_ARR=$(./bin_array_par)
    echo "Array,$t,$T_ARR" >> benchmark_data.csv
    
    T_SORT=$(./bin_sort_par)
    echo "Sort,$t,$T_SORT" >> benchmark_data.csv
done

echo "Done! Results saved to benchmark_data.csv"