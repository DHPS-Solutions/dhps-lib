#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "../sort.h"

#define SIZE (10 * 1000000)

static inline bool cmp(const void *a, const void *b)
{
    return *(int *)a < *(int *)b;
}

static inline int cmp_2(const void *a, const void *b)
{
    return *(int *)a > *(int *)b;
}

void fill_random(int *data, int len)
{
    for (int i = 0; i < len; i++)
        *(data + i) = (int)rand();
}

void fill_sorted(int *data, int len)
{
    for (int i = 0; i < len; i++)
        *(data + i) = i;
}

void fill_reverse_sorted(int *data, int len)
{
    for (int i = 0; i < len; i++)
        *(data + i) = len - i;
}

void fill_half_random(int *data, int len)
{
    int j = len >> 1;
    int k = len / 3;
    for (int i = 0; i < len; i++) {
        if (i % 2 == 0)
            *(data + i) = rand();
        else if (i % 3 == 0)
            *(data + i) = k;
        else
            *(data + i) = j;
    }
}

void fill_small_range(int *data, int len)
{
    for (int i = 0; i < len; i++)
        *(data + i) = rand() % 5;
}

void test_sorted(int *data, int len)
{
    for (int i = 1; i < len; i++) {
        if (*(data + i) < *(data + i - 1)) {
            printf("ERROR! Array is not sorted.\n");
            break;
        }
        if (i == len - 1)
            printf("SUCCESS! Array is sorted.\n");
    }
}

long long sum_array(int *data, int len)
{
    long long sum = 0;
    for (int i = 0; i < len; i++) {
        sum += *(data + i);
    }
    return sum;
}

void copy(int *data, int *copy, int len)
{
    for (int i = 0; i < len; i++)
        *(copy + i) = *(data + i);
}

void test_values(long long sum1, long long sum2)
{
    if (sum1 == sum2)
        printf("SUCCESS! The sum before sorting: %lld is equal to the sum after sorting %lld\n", sum1, sum2);
    else
        printf("ERROR! The sum before sorting: %lld is not equal to the sum after sorting %lld\n", sum1, sum2);
}

void qs_run_sort_with_time(int *data)
{
    clock_t t;
    double elapsed;
    long long sum_before = 0;
    long long sum_after = 0;
    sum_before = sum_array(data, SIZE);
    test_sorted(data, SIZE);
    printf("Started sorting\n");
    t = clock();
    quicksort(data, SIZE, sizeof(int), cmp);
    t = clock() - t;
    double time_taken = ((double)t)/(CLOCKS_PER_SEC/1000);
    printf("Time taken was: %fms\n", time_taken);
    test_sorted(data, SIZE);
    sum_after = sum_array(data, SIZE);
    test_values(sum_before, sum_after);
}

void std_qs_run_sort_with_time(int *data)
{
    clock_t t;
    double elapsed;
    long long sum_before = 0;
    long long sum_after = 0;
    sum_before = sum_array(data, SIZE);
    test_sorted(data, SIZE);
    printf("Started sorting\n");
    t = clock();
    qsort(data, SIZE, sizeof(int), cmp_2);
    t = clock() - t;
    double time_taken = ((double)t)/(CLOCKS_PER_SEC/1000);
    printf("Time taken was: %fms\n", time_taken);
    test_sorted(data, SIZE);
    sum_after = sum_array(data, SIZE);
    test_values(sum_before, sum_after);
}

void rec_qs_run_sort_with_time(int *data)
{
    clock_t t;
    double elapsed;
    long long sum_before = 0;
    long long sum_after = 0;
    sum_before = sum_array(data, SIZE);
    test_sorted(data, SIZE);
    printf("Started sorting\n");
    t = clock();
    quicksort_rec(data, SIZE, sizeof(int), cmp);
    t = clock() - t;
    double time_taken = ((double)t)/(CLOCKS_PER_SEC/1000);
    printf("Time taken was: %fms\n", time_taken);
    test_sorted(data, SIZE);
    sum_after = sum_array(data, SIZE);
    test_values(sum_before, sum_after);
}

void heap_sort_run_sort_with_time(int *data)
{
    clock_t t;
    double elapsed;
    long long sum_before = 0;
    long long sum_after = 0;
    sum_before = sum_array(data, SIZE);
    test_sorted(data, SIZE);
    printf("Started sorting\n");
    t = clock();
    heap_sort(data, SIZE, sizeof(int), cmp);
    t = clock() - t;
    double time_taken = ((double)t)/(CLOCKS_PER_SEC/1000);
    printf("Time taken was: %fms\n", time_taken);
    test_sorted(data, SIZE);
    sum_after = sum_array(data, SIZE);
    test_values(sum_before, sum_after);
}

int main()
{
    int *data = (int *)(malloc(SIZE * sizeof(int)));
    int *copy_arr = (int *)(malloc(SIZE * sizeof(int)));
    printf("--------------------------------------------------------\n");
    printf("Random Numbers: %d million. \n\n", SIZE / 1000000);
    fill_random(data, SIZE);
    copy(data, copy_arr, SIZE);
    printf("DHPS-Sort: \n");
    qs_run_sort_with_time(copy_arr);
    copy(data, copy_arr, SIZE);
    printf("\nQsort: \n");
    std_qs_run_sort_with_time(copy_arr);
    copy(data, copy_arr, SIZE);
    printf("\nRecursive Qsort: \n");
    rec_qs_run_sort_with_time(copy_arr);
    // printf("\nHeap sort: \n");
    // copy(data, copy_arr, SIZE);
    // heap_sort_run_sort_with_time(copy_arr);
    printf("--------------------------------------------------------\n");
    printf("--------------------------------------------------------\n");
    printf("Sorted Numbers: %d million. \n\n", SIZE / 1000000);
    fill_sorted(data, SIZE);
    copy(data, copy_arr, SIZE);
    printf("DHPS-Sort: \n");
    qs_run_sort_with_time(copy_arr);
    copy(data, copy_arr, SIZE);
    printf("\nQsort: \n");
    std_qs_run_sort_with_time(copy_arr);
    printf("--------------------------------------------------------\n\n");

    printf("--------------------------------------------------------\n");
    printf("Reverse sorted Numbers: %d million. \n\n", SIZE / 1000000);
    fill_reverse_sorted(data, SIZE);
    copy(data, copy_arr, SIZE);
    printf("DHPS-Sort: \n");
    qs_run_sort_with_time(copy_arr);
    copy(data, copy_arr, SIZE);
    printf("\nQsort: \n");
    std_qs_run_sort_with_time(copy_arr);
    printf("--------------------------------------------------------\n\n");

    printf("--------------------------------------------------------\n");
    printf("Half random Numbers: %d million. \n\n", SIZE / 1000000);
    fill_half_random(data, SIZE);
    copy(data, copy_arr, SIZE);
    printf("DHPS-Sort: \n");
    qs_run_sort_with_time(copy_arr);
    copy(data, copy_arr, SIZE);
    printf("\nQsort: \n");
    std_qs_run_sort_with_time(copy_arr);
    printf("--------------------------------------------------------\n\n");

    printf("--------------------------------------------------------\n");
    printf("Low range Numbers: %d million. \n\n", SIZE / 1000000);
    fill_small_range(data, SIZE);
    copy(data, copy_arr, SIZE);
    printf("DHPS-Sort: \n");
    qs_run_sort_with_time(copy_arr);
    copy(data, copy_arr, SIZE);
    printf("\nQsort: \n");
    std_qs_run_sort_with_time(copy_arr);
    printf("--------------------------------------------------------\n\n");
    free(data);
    free(copy_arr);
    return 0;
}