#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "../sort.h"

#define SIZE (5 * 1000000)
double arr[SIZE];
double arr2[SIZE];

static inline bool cmp(const void *a, const void *b)
{
    return *(double *)a < *(double *)b;
}

static inline int cmp_2(const void *a, const void *b)
{
    return *(double *)a > *(double *)b;
}

int main()
{
    for (int i = 0; i < SIZE; i++) {
        double num = (double)(rand());
        *(arr + i) = num;
        *(arr2 + i) = num;
    }

    clock_t t;
    t = clock();
    quicksort(arr, SIZE, sizeof(double), cmp);
    t = clock() - t;

    double time_taken = ((double)t)/(CLOCKS_PER_SEC/1000);

    printf("My Time: %fms\n", time_taken);

    t = clock();
    qsort(arr2, SIZE, sizeof(double), cmp_2);
    t = clock() - t;

    time_taken = ((double)t)/(CLOCKS_PER_SEC/1000);

    // for (int i = 0; i < SIZE - 1; i++)
    //     if (*(arr + i) > *(arr + i + 1))
    //         printf("ERROR! %d > %d\n", *(arr + i), *(arr + i + 1));

    printf("Std Time: %fms\n", time_taken);

    return 0;
}