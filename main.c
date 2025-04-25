#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bubble_sort.h"
#include "selection_sort.h"
#include "insertion_sort.h"
#include "merge_sort.h"
#include "quick_sort.h"
#include "shell_sort.h"

#define MAX_WORD_LENGTH 100

void read_numbers(const char *filename, int *arr, int n) {
    FILE *fp = fopen(filename, "r");
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%d", &arr[i]);
    }
    fclose(fp);
}

void read_words(const char *filename, char **arr, int n) {
    FILE *fp = fopen(filename, "r");
    for (int i = 0; i < n; i++) {
        arr[i] = malloc(MAX_WORD_LENGTH);
        fscanf(fp, "%s", arr[i]);
    }
    fclose(fp);
}

void free_words(char **arr, int n) {
    for (int i = 0; i < n; i++) {
        free(arr[i]);
    }
}

void benchmark_sort_int(void (*sort_func)(int *, int), int *data, int n, const char *name) {
    int *copy = malloc(n * sizeof(int));
    memcpy(copy, data, n * sizeof(int));
    clock_t start = clock();
    sort_func(copy, n);
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    size_t memory = n * sizeof(int);
    printf("%-15s | %7d elemen | %7.3f detik | %7zu byte\n", name, n, elapsed, memory);
    free(copy);
}

void benchmark_sort_str(void (*sort_func)(char **, int), char **data, int n, const char *name) {
    char **copy = malloc(n * sizeof(char *));
    size_t total_memory = n * sizeof(char *);
    for (int i = 0; i < n; i++) {
        copy[i] = strdup(data[i]);
        total_memory += strlen(copy[i]) + 1;
    }
    clock_t start = clock();
    sort_func(copy, n);
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("%-15s | %7d elemen | %7.3f detik | %7zu byte\n", name, n, elapsed, total_memory);
    for (int i = 0; i < n; i++) free(copy[i]);
    free(copy);
}

// Wrapper untuk merge sort dan quick sort angka
void merge_sort_wrapper(int *arr, int n) {
    merge_sort_int(arr, 0, n - 1);
}
void quick_sort_wrapper(int *arr, int n) {
    quick_sort_int(arr, 0, n - 1);
}

// Wrapper untuk merge sort dan quick sort string
void merge_sort_wrapper_str(char **arr, int n) {
    merge_sort_str(arr, 0, n - 1);
}
void quick_sort_wrapper_str(char **arr, int low, int high) {
    if (low < high) {
        char *pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (strcmp(arr[j], pivot) <= 0) {
                i++;
                char *temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        char *temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        int pi = i + 1;
        quick_sort_wrapper_str(arr, low, pi - 1);
        quick_sort_wrapper_str(arr, pi + 1, high);
    }
}

void benchmark_quick_sort_str(char **data, int n) {
    char **copy = malloc(n * sizeof(char *));
    size_t total_memory = n * sizeof(char *);
    for (int i = 0; i < n; i++) {
        copy[i] = strdup(data[i]);
        total_memory += strlen(copy[i]) + 1;
    }
    clock_t start = clock();
    quick_sort_wrapper_str(copy, 0, n - 1);
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("%-15s | %7d elemen | %7.3f detik | %7zu byte\n", "Quick Sort", n, elapsed, total_memory);
    for (int i = 0; i < n; i++) free(copy[i]);
    free(copy);
}

int main() {
    const int sizes[] = {10000, 50000, 100000, 250000, 500000, 1000000, 1500000, 2000000};
    const int size_count = sizeof(sizes) / sizeof(sizes[0]);

    int *data_numbers = malloc(2000000 * sizeof(int));
    char **data_words = malloc(2000000 * sizeof(char *));

    read_numbers("data_angka.txt", data_numbers, 2000000);
    read_words("data_kata.txt", data_words, 2000000);

    for (int i = 0; i < size_count; i++) {
        int n = sizes[i];
    
        printf("\n>>> Ukuran data: %d angka\n", n);
        printf("%-15s | %7s       | %14s | %12s\n", "Algoritma", "Jumlah", "Waktu", "Memori");
        benchmark_sort_int(bubble_sort_int, data_numbers, n, "Bubble Sort");
        benchmark_sort_int(selection_sort_int, data_numbers, n, "Selection Sort");
        benchmark_sort_int(insertion_sort_int, data_numbers, n, "Insertion Sort");
        benchmark_sort_int(shell_sort_int, data_numbers, n, "Shell Sort");
        benchmark_sort_int(merge_sort_wrapper, data_numbers, n, "Merge Sort");
        benchmark_sort_int(quick_sort_wrapper, data_numbers, n, "Quick Sort");
    
        printf("\n>>> Ukuran data: %d kata\n", n);
        printf("%-15s | %7s       | %14s | %12s\n", "Algoritma", "Jumlah", "Waktu", "Memori");
        benchmark_sort_str(bubble_sort_str, data_words, n, "Bubble Sort");
        benchmark_sort_str(selection_sort_str, data_words, n, "Selection Sort");
        benchmark_sort_str(insertion_sort_str, data_words, n, "Insertion Sort");
        benchmark_sort_str(shell_sort_str, data_words, n, "Shell Sort");
        benchmark_sort_str(merge_sort_wrapper_str, data_words, n, "Merge Sort");
        benchmark_quick_sort_str(data_words, n);
    }
    
    free(data_numbers);
    free_words(data_words, 2000000);
    free(data_words);
    return 0;
}
