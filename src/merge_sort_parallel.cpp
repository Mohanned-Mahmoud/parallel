#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <cstdlib>

const int SIZE = 5000000; // 5 Million elements

void merge(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (i = 0; i < k; ++i) arr[left + i] = temp[i];
}

void merge_sort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        // Use Tasks for recursion as per your uploaded file
        if (right - left > 10000) { // Only parallelize large chunks to avoid overhead
            #pragma omp task
            merge_sort(arr, left, mid);
            #pragma omp task
            merge_sort(arr, mid + 1, right);
            #pragma omp taskwait
        } else {
            merge_sort(arr, left, mid);
            merge_sort(arr, mid + 1, right);
        }
        merge(arr, left, mid, right);
    }
}

int main() {
    std::vector<int> arr(SIZE);
    for(int i=0; i<SIZE; i++) arr[i] = rand() % 1000;

    auto start = std::chrono::high_resolution_clock::now();
    #pragma omp parallel
    {
        #pragma omp single
        merge_sort(arr, 0, SIZE - 1);
    }
    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

    std::cout << duration << std::endl;
    return 0;
}