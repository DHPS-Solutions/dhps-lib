#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "sort.h"

void insertion_sort(const void* base, int left_idx, int right_idx, size_t size, __compare_fn_t cmp)
{
    char *base_ptr = (char *)base;
    char *left_ptr = base_ptr + size;
    char *right_ptr = base_ptr + size * right_idx;

    char change[size];
    char *j;

    while (left_ptr <= right_ptr) {
        for (size_t i = 0; i < size; i++)
            *(change + i) = *(left_ptr + i);
        j = left_ptr - size;
        while (cmp(change, j) && j >= base_ptr) {
            byte_assert(j + size, j, size);
            j -= size;
        }
        byte_assert(j + size, change, size);

        left_ptr += size;
    }
}