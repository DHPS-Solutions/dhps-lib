#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "sort.h"

/* Nice clean pointer bubblesort, never fully O(n^2). */
static void __bubble_sort(char *left, char *right, size_t size, compare_fn_t cmp)
{
    for (char *i = right; i > left; i -= size) // Start at the right.
        for (char *j = left; j < i; j += size) // Check all elements before i if they are larger.
            if ((*cmp)(i, j))
                BYTE_SWAP(j, i, size); // Swap if larger.
}

/* Nice clean pointer insertion sort, never fully O(n^2). */
static void __insertion_sort(char *left, char *right, size_t size, compare_fn_t cmp)
{
    // First loop that semi sorts array, moves smallest element to first position.
    for (char* i = left + size; i <= right; i += size)
        if (cmp(i, left))
            BYTE_SWAP(i, left, size);

    // Second loop sorts all elements, extremely fast if array is almosot already sorted.
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

/* Standard sorting of three values. */
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

/* Magical block partitioning based on this research paper: https://arxiv.org/pdf/1604.06697.pdf */
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

    if (right - left + size > 2 * block_size) { // Probably inefficient code, but no
        l = left + (size * 2);                  // need to init if the loop doesn't run.
        r = right - size;

        char *offset_left[block_size];          // Left buffer.
        char *offset_right[block_size];         // Right buffer.
        char **num_left = offset_left;          // Tmp pointer to left buffer.
        char **num_right = offset_right;        // Tmp pointer to right buffer.

        do {                                    // Main loop for partioning.
            if (num_left == offset_left) {      // If the left buffer is empty start loop.
                char *pd = l;                   // Tmp value for the left-most value.
                do {                            
                    *num_left = pd;             // Set the the current address in the buffer.
                    num_left += cmp(piv, pd);   // If the value is larger than current piv, go to the next pointer.
                    pd += size;                 // Go to the next value to the right.
                } while (pd < l + block_size);  // Continue until movement is equal to blocksize.
            }
            if (num_right == offset_right) {    // If the left buffer is empty start loop.
                char *pd = r;                   // Tmp value for the right-most value.
                do {
                    *num_right = pd;            // Set the the current address in the buffer.
                    num_right += cmp(pd, piv);  // If the value is larger than current piv, go to the next pointer.
                    pd -= size;                 // Go to the next value to the left.
                } while (pd > r - block_size);  // Continue until movement is equal to blocksize.
            }

            // Get the size of the smallest buffer and move that many places to the left in both buffers.
            int min = min(num_left - offset_left, num_right - offset_right); 
            num_left -= min;
            num_right -= min;
            
            // Swap the pointers so the pointers to larger values are on the right buffer and smaller on the left buffer.
            for (int i = 0; i < min; i++)
                BYTE_SWAP(*(num_left + i), *(num_right + i), size);

            // If the pointers are equal to the buffer, this block is correct for that side and we move to the next block.
            if (num_left == offset_left)
                l += block_size;
            if (num_right == offset_right)
                r -= block_size;
        } while(r - l + size > 2 * block_size); 
        l -= size;
        r += size;
    } else {
        l = left + size;
        r = right;
    }

    // Hoare's partition, slightly rewritten.
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
    char *base_ptr = (char *)base; // Cast the array to an array of bytes.
    char *bound_ptr = base_ptr + size * (nmemb - 1); // Create a pointer to the last byte.
    __insertion_sort(base_ptr, bound_ptr, size, cmp);    
}

inline void bubble_sort(const void *base, size_t nmemb, size_t size, compare_fn_t cmp)
{
    char *base_ptr = (char *)base; // Cast the array to an array of bytes
    char *bound_ptr = base_ptr + size * (nmemb - 1); // Create a pointer to the last byte.
    __bubble_sort(base_ptr, bound_ptr, size, cmp);
}

static void __quicksort_rec(char *left, char *right, size_t size, compare_fn_t cmp)
{
    if (right - left > RUN_INSERTION) {
        char *piv; // Create pointer for the pivot/split-point.
        __block_partition(left, right, &piv, size, cmp); // Partition the pointer.
        __quicksort_rec(piv + size, right, size, cmp);
        __quicksort_rec(left, piv - size, size, cmp);
    } else {
        __insertion_sort(left, right, size, cmp);
    }
}

inline void quicksort(const void *base, size_t nmemb, size_t size, compare_fn_t cmp) 
{
    if (!nmemb) // Return if there are no items.
        return;
    
    char *base_ptr = (char *)base; // Cast the array to an array of bytes.
    char *left_ptr = base_ptr;
    char *right_ptr = base_ptr + size * nmemb; // Create a pointer to the last byte.
    __quicksort_rec(left_ptr, right_ptr, size, cmp);
}