#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "sort.h"

static void __bubble_sort(const void *base, size_t left_idx, size_t right_idx, size_t size, compare_fn_t cmp)
{
    char *base_ptr = (char *)base;
    size_t max = size * right_idx;
    for (size_t i = max; i > 0; i -= size)
        for (size_t j = 0; j < i; j += size)
            if ((*cmp)(base_ptr + i, base_ptr + j))
                BYTE_SWAP(base_ptr + j, base_ptr + i, size);  
}

static void __insertion_sort(char *left, char *right, size_t size, compare_fn_t cmp)
{
    for (char* i = left + size; i <= right; i += size)
        if (cmp(i, left))
            BYTE_SWAP(i, left, size);

    for (char* i = left + 2 * size; i <= right; i += size) {
        char high[size];
        BYTE_ASSERT(high, i, size);
        char* j = i - size;
        while (cmp(high, j)) {
            BYTE_ASSERT(j + size, j, size);
            j -= size;
        }
        BYTE_ASSERT(j + size, high, size);
    }
}

static inline void __median_three(char *left, char *mid, char *right, size_t size, compare_fn_t cmp)
{
    if ((*cmp)(mid, left))
        BYTE_SWAP(mid, left, size);

    if ((*cmp)(right, mid)) {
        BYTE_SWAP(right, mid, size);
        if ((*cmp)(mid, left)) {
            BYTE_SWAP(mid, left, size);
        }
    }
}

static void __block_partition(char *left, char *right, char **pivot, size_t size, compare_fn_t cmp)
{
    char *mid = left + FIND_MID(right, left, size);
    char piv[size];
    BYTE_ASSERT(piv, mid, size);
    BYTE_ASSERT(mid, left + size, size);
    __median_three(left, piv, right, size, cmp);
    BYTE_ASSERT(left + size, piv, size);

    char *l;
    char *r;

    int block_size = BLOCK * size;

    if (right - left > (block_size * 2) + 3 * size) {
        l = left + (size * 2);
        r = right - size;

        char *offset_left[block_size];
        char *offset_right[block_size];
        char **offset_dif_left = offset_left;
        char **offset_dif_right = offset_right;

        do {
            if (offset_dif_left == offset_left) {
                char *pd = l;
                do {
                    *offset_dif_left = pd;
                    offset_dif_left += cmp(piv, pd);
                    pd += size;
                } while (pd < l + block_size);
            }
            if (offset_dif_right == offset_right) {
                char* pd = r;
                do {
                    *offset_dif_right = pd;
                    offset_dif_right += cmp(pd, piv);
                    pd -= size;    
                } while (pd > r - block_size);
            }

            int min = min(offset_dif_left - offset_left, offset_dif_right - offset_right);
            offset_dif_left -= min;
            offset_dif_right -= min;
            
            for (int i = 0; i < min; i++)
                BYTE_SWAP(*(offset_dif_left + i), *(offset_dif_right + i), size);

            if (offset_dif_left == offset_left)
                l += block_size;
            if (offset_dif_right == offset_right)
                r -= block_size;
        } while(r - l > 2 * block_size);
        l -= size;
        r += size;
    } else {
        l = left + size;
        r = right;
    }

    do {
        do l += size; 
        while((*cmp)(l, piv));
        do r -= size;
        while ((*cmp)(piv, r));

        if (l >= r)
            break;

        BYTE_SWAP(l, r, size);
    } while(true);

    BYTE_ASSERT(left + size, r, size);
    BYTE_ASSERT(r, piv, size);
    *pivot = r;
}

inline void insertion_sort(const void *base, size_t nmemb, size_t size, compare_fn_t cmp)
{
    char *base_ptr = (char *)base;
    char *bound_ptr = base_ptr + size * (nmemb - 1);
    __insertion_sort(base_ptr, bound_ptr, size, cmp);    
}

inline void bubble_sort(const void *base, size_t nmemb, size_t size, compare_fn_t cmp)
{
    __bubble_sort(base, 0, nmemb - 1, size, cmp);
}

static void __quicksort_rec(char *left, char *right, size_t size, compare_fn_t cmp)
{
    if (right - left > RUN_INSERTION) {
        char *piv;
        __block_partition(left, right, &piv, size, cmp);
        __quicksort_rec(piv + size, right, size, cmp);
        __quicksort_rec(left, piv - size, size, cmp);
    } else {
        __insertion_sort(left, right, size, cmp);
    }
}

inline void quicksort(const void* base, size_t nmemb, size_t size, compare_fn_t cmp) 
{
    if (!nmemb)
        return;
    
    char *base_ptr = (char *)base;
    char *left_ptr = base_ptr;
    char *right_ptr = base_ptr + size * nmemb;
    __quicksort_rec(left_ptr, right_ptr, size, cmp);
}