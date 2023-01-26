#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "sort.h"

bool cmp(const void *a, const void *b)
{   
    if (*(int *)a == 0)
        printf("%d < %d\n", *(int *)a, *(int *)b);
    
    return *(int *)a < *(int *)b;
}

int cmp2(const void *a, const void *b)
{
    return *(int *)a > *(int *)b;
}

int main()
{
    int arr[256];
    int copy[256];

    for (int i = 0; i < 256; i++) {
        int num = rand() % 100;
        *(copy + i) = num;
        *(arr + i) = num;
    }

    printf("UNSORTED: \n");
    for (int i = 0; i < 256; i++)
        printf("%d \n", *(arr + i));

    insertion_sort(arr, 0, 255, sizeof(int), cmp);

    qsort(copy, 256, sizeof(int), cmp2);

    printf("SORTED: \n");
    for (int i = 0; i < 256; i++) {
        printf("Copy: %d \n", *(copy + i));
        printf("Arr: %d \n", *(arr + i));
    }

    printf("Sorted? \n");
    int i;
    for (i = 2; i < 256; i++)
        if (*(arr + i) != *(copy + i))
            break;
    
    printf("%d", i);

    return 0;
}