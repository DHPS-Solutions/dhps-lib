#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "sort.h"

static void __insert_sort(const void *base, size_t left_idx, size_t right_idx, size_t size, compare_fn_t cmp)
{
    char *base_ptr = (char *)base;
    char *left_ptr = base_ptr + size;
    char *right_ptr = base_ptr + size * right_idx;

    char change[size];
    char *j;

    while (left_ptr <= right_ptr) {
        BYTE_ASSERT(change, left_ptr, size);
        j = left_ptr - size;
        while (cmp(change, j) && j >= base_ptr) {
            BYTE_ASSERT(j + size, j, size);
            j -= size;
        }
        BYTE_ASSERT(j + size, change, size);

        left_ptr += size;
    }
}


static void __median_three(char *left, char *mid, char *right, size_t size, compare_fn_t cmp)
{
    if (cmp(mid, left))
        BYTE_SWAP(mid, left, size);

    if (cmp(right, mid)) {
        BYTE_SWAP(right, mid, size);
        if (cmp(mid, left)) {
            BYTE_SWAP(mid, left, size);
        }
    }
}

static void __partition(char *left, char *right, char **pivot, size_t size, compare_fn_t cmp)
{
    char *l = left + size;
    char *r = right;
    char *mid = left + FIND_MID(right, left, size);
    __median_three(left, mid, right, size, cmp);

    char piv[size];
    BYTE_ASSERT(piv, mid, size);
    BYTE_ASSERT(mid, l, size);
    BYTE_ASSERT(l, piv, size);
    BYTE_SWAP(mid, right - size, size);

    while (true) {
        do l += size; 
        while(cmp(l, piv));
        do r -= size;
        while (cmp(piv, r));

        if (l >= r)
            break;

        BYTE_SWAP(l, r, size);
    }

    BYTE_ASSERT(left + size, r, size);
    BYTE_ASSERT(r, piv, size);
    *pivot = r;
}

inline void insertion_sort(const void *base, size_t nmemb, size_t size, compare_fn_t cmp)
{
    __insert_sort(base, 0, nmemb - 1, size, cmp);    
}

static void __qsort_rec(char *left, char *right, size_t size, compare_fn_t cmp)
{
    if (right - left >= RUN_INSERTION(size)) {
        char *piv;
        __partition(left, right, &piv, size, cmp);
        __qsort_rec(piv + size, right, size, cmp);
        __qsort_rec(left, piv - size, size, cmp);
    } else {
        __insert_sort(left, 0, (right - left + size) / size, size, cmp);
    }
}

inline void quicksort(const void* base, size_t nmemb, size_t size, compare_fn_t cmp) 
{
    char *base_ptr = (char *)base;
    char *left_ptr = base_ptr;
    char *right_ptr = base_ptr + size * (nmemb - 1);
    __qsort_rec(left_ptr, right_ptr, size, cmp);
}