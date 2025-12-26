@echo off
setlocal enabledelayedexpansion

:: 1. Create Bin Folder if not exists
if not exist "bin" mkdir bin

:: 2. Compile Everything
echo Compiling Benchmarks...
g++ -O2 src/matrix_mult.cpp -o bin/matrix_seq.exe
g++ -O2 -fopenmp src/matrix_mult_parallel.cpp -o bin/matrix_par.exe

g++ -O2 src/array_processing.cpp -o bin/array_seq.exe
g++ -O2 -fopenmp src/array_processing_parallel.cpp -o bin/array_par.exe

g++ -O2 src/merge_sort.cpp -o bin/sort_seq.exe
g++ -O2 -fopenmp src/merge_sort_parallel.cpp -o bin/sort_par.exe

:: 3. Prepare CSV Output
echo Benchmark,Threads,Time > benchmark_data.csv

:: 4. Run Sequential Baseline
echo Running Sequential...
for /f %%i in ('bin\matrix_seq.exe') do set T_MAT=%%i
echo Matrix,1,!T_MAT! >> benchmark_data.csv

for /f %%i in ('bin\array_seq.exe') do set T_ARR=%%i
echo Array,1,!T_ARR! >> benchmark_data.csv

for /f %%i in ('bin\sort_seq.exe') do set T_SORT=%%i
echo Sort,1,!T_SORT! >> benchmark_data.csv

:: 5. Run Parallel Tests
echo Running Parallel Tests...
for %%t in (2 4 8) do (
    echo Testing with %%t Threads...
    set OMP_NUM_THREADS=%%t
    
    for /f %%i in ('bin\matrix_par.exe') do set T_MAT=%%i
    echo Matrix,%%t,!T_MAT! >> benchmark_data.csv
    
    for /f %%i in ('bin\array_par.exe') do set T_ARR=%%i
    echo Array,%%t,!T_ARR! >> benchmark_data.csv
    
    for /f %%i in ('bin\sort_par.exe') do set T_SORT=%%i
    echo Sort,%%t,!T_SORT! >> benchmark_data.csv
)

echo Done! Data saved to benchmark_data.csv
pause