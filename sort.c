#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <tgmath.h>

#include "heap.h"
#include "stack.h"
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
    for (char* i = left + (size << 1); i <= right; i += size) {
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
static inline char *__median_three(char *left, char *right, size_t size, compare_fn_t cmp)
{
    char *mid = left + FIND_MID(right, left, size);

    if (cmp(mid, left)) {
        if (cmp(right, left)) {
            if (cmp (right, mid)) { 
                BYTE_SWAP(left, right, size);
            } else { 
                THREE_BYTE_ROTATE(left, mid, right, size);
            }
        }
        else { 
            BYTE_SWAP(left, mid, size); 
        }
    }
    else {
        if ((*cmp)(right, mid)) {
            if (right < left) { 
                THREE_BYTE_ROTATE(right, mid, left, size);
            } else { 
                BYTE_SWAP(mid, right, size); 
            }
        }
    }

    return mid;
}

static char *__median_five(char *left, char *right, size_t size, compare_fn_t cmp)
{   
    char* mid = left + FIND_MID(right, left, size);

    char* left_mid = left + FIND_FIRST_QUART(right, left, size);
    
    char* right_mid = left + FIND_THIRD_QUART(right, left, size);

    if ((*cmp)(left_mid, left))
        BYTE_SWAP(left, left_mid, size);
    if ((*cmp)(right, right_mid))
        BYTE_SWAP(right_mid, right, size);
    if ((*cmp)(right_mid, left))
        BYTE_SWAP(left, right_mid, size);
    if ((*cmp)(right, left_mid))
        BYTE_SWAP(left_mid, right, size);
    if ((*cmp)(right_mid, mid))
        BYTE_SWAP(mid, right_mid, size);
    if ((*cmp)(mid, left_mid))
        BYTE_SWAP(left_mid, mid, size);
    if ((*cmp)(right_mid, mid))
        BYTE_SWAP(right_mid, mid, size);

    return mid;
}

static char *__median_five_of_three(char *left, char *right, size_t size, compare_fn_t cmp)
{
    char *t_first_quart = left + FIND_FIRST_QUART(right, left, size);
    char *t_mid = left + FIND_MID(right, left, size);
    char *t_third_quart = left + FIND_THIRD_QUART(right, left, size);

    char *left_mid = __median_five(left, t_first_quart, size, cmp);

    char *mid = __median_five(t_first_quart, t_mid, size, cmp);

    char *right_mid = __median_five(t_mid, t_third_quart, size, cmp);

    char *last = __median_five(t_third_quart, right, size, cmp);

    if ((*cmp)(left_mid, left))
        BYTE_SWAP(left, left_mid, size);
    if ((*cmp)(right, right_mid))
        BYTE_SWAP(right_mid, right, size);
    if ((*cmp)(right_mid, left))
        BYTE_SWAP(left, right_mid, size);
    if ((*cmp)(right, left_mid))
        BYTE_SWAP(left_mid, right, size);
    if ((*cmp)(right_mid, mid))
        BYTE_SWAP(mid, right_mid, size);
    if ((*cmp)(mid, left_mid))
        BYTE_SWAP(left_mid, mid, size);
    if ((*cmp)(right_mid, mid))
        BYTE_SWAP(right_mid, mid, size);
    
    return mid;
}

/* Magical block partitioning based on this research paper: https://arxiv.org/pdf/1604.06697.pdf */
static void __block_partition(char *left, char *right, char **pivot_l, char **pivot_r, size_t size, compare_fn_t cmp)
{
    char *mid;
    char piv[size];
    int diff = right - left;

    if (diff > size << 13) {
        mid = __median_five_of_three(left, right, size, cmp);
    } else if (diff > (size << 2) + 1) { // If the array is larger than 5 elements, use median of five.
        mid = __median_five(left, right, size, cmp);
    } else {
        mid = __median_three(left, right, size, cmp);
    }

    BYTE_ASSERT(piv, mid, size);
    BYTE_ASSERT(mid, left + size, size);
    BYTE_ASSERT(left + size, piv, size);

    char *l;
    char *r;

    int block_size = BLOCK * size;

    if (diff > (block_size << 1) + (size << 1)) { // Probably inefficient code, but no
        l = left + (size << 1);                  // need to init if the loop doesn't run.
        r = right - size;

        char *offset_left[block_size];          // Left buffer.
        char *offset_right[block_size];         // Right buffer.
        char **num_left = offset_left;          // Tmp pointer to left buffer.
        char **num_right = offset_right;        // Tmp pointer to right buffer.

        do {                                    // Main loop for partioning.
            char *l_lim = l + block_size;       // Limit for the left buffer.
            if (num_left == offset_left) {      // If the left buffer is empty start loop.
                char *pd = l;                   // Tmp value for the left-most value.
                do {                            
                    *num_left = pd; 
                    bool pred = true;
                    if (*piv == *pd) {
                        if (size == 1) {
                            pred = true;
                        } else {
                            BYTES_EQUAL(piv + 1, pd + 1, size - 1, pred);
                        }
                    } else {
                        pred = false;        
                    }
                    num_left += pred || cmp(piv, pd);   // If the value is larger than current piv, go to the next pointer.
                    pd += size;                 // Go to the next value to the right.
                } while (pd <= l_lim);  // Continue until movement is equal to blocksize.
            }

            char *r_lim = r - block_size;       // Limit for the right buffer.
            if (num_right == offset_right) {    // If the left buffer is empty start loop.
                char *pd = r;                   // Tmp value for the right-most value.
                do {
                    *num_right = pd;
                    bool pred = true;
                    if (*piv == *pd) {
                        if (size == 1) {
                            pred = true;
                        } else {
                            BYTES_EQUAL(piv + 1, pd + 1, size - 1, pred);
                        }
                    } else {
                        pred = false;        
                    }
                    num_right += pred || cmp(pd, piv);  // If the value is larger than current piv, go to the next pointer.
                    pd -= size;                 // Go to the next value to the left.
                } while (pd >= r_lim);  // Continue until movement is equal to blocksize.
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
        } while(r - l >= block_size << 1);
        
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

    // Move all elements equal to the pivot to the left.
    // TODO: Fix this.
    char *piv_l = r;

    if (right - r > size << 13) {
        char *pd = r + size;
        int k = 0;
        while(k != 3 && pd <= right) {
            if (cmp(pd, piv)) {
                k = 0;
                r += size;
                BYTE_SWAP(pd, r, size);
            }
            
            k++;
            pd += size;
        }
    }
    

    BYTE_ASSERT(left + size, r, size);
    BYTE_ASSERT(r, piv, size);
    *pivot_l = piv_l;
    *pivot_r = r;
}

static void __partition(char *left, char *right, char **pivot, size_t size, compare_fn_t cmp)
{
    char *mid;
    char piv[size];
    int diff = right - left;

    if (diff > size << 7) { // If the array is larger than 128 elements, use median of five.
        mid = __median_five_of_three(left, right, size, cmp); 
    } else if (diff > (size << 2) + 1) { // If the array is larger than 32 elements, use median of five.
        mid = __median_five(left, right, size, cmp);
    } else {
        mid = __median_three(left, right, size, cmp);
    }

    char *l = left + size;
    char *r = right;

    BYTE_ASSERT(piv, mid, size);
    BYTE_ASSERT(mid, l, size);
    BYTE_ASSERT(l, piv, size);
    BYTE_SWAP(mid, right - size, size);

    do {
        do l += size;
        while(cmp(l, piv));
        
        do r -= size;
        while (cmp(piv, r));

        if (l >= r)
            break;

        BYTE_SWAP(l, r, size);
    } while(true);

    BYTE_ASSERT(left + size, r, size);
    BYTE_ASSERT(r, piv, size);
    *pivot = r;
}

void __heap_sort(char *left, char *right, size_t size, compare_fn_t cmp)
{
    struct heap_t *heap = heap_malloc(cmp);
    char *p = left;
    char elems[(right - left) / size][size];
    int i = 0;
    while (p <= right) {
        BYTE_ASSERT(*(elems + i), p, size);
        heap_push(heap, *(elems + i++));
        p += size;
    }

    p = right;

    while (p >= left) {
        char *elem = (char *)heap_pop(heap);
        BYTE_SWAP(p, elem, size);
        p -= size;
    }

    heap_free(heap);
}

static void __quicksort_rec(char *left, char *right, size_t size, compare_fn_t cmp)
{   
    if (right - left >= RUN_INSERTION * size) {
        char *piv; // Create pointer for the pivot/split-point.
        char *piv_l;
        __block_partition(left, right, &piv, &piv_l, size, cmp); // Partition the pointer.
        __quicksort_rec(piv + size, right, size, cmp);
        __quicksort_rec(left, piv - size, size, cmp);
    } else {
        __insertion_sort(left, right, size, cmp);
    }
}

static void __quicksort(char *left, char *right, size_t size, compare_fn_t cmp)
{   
    const int depth_limit = 2 * ilogb((double)(right - left)) + 3;
    const int ins_limit = RUN_INSERTION * size;
    int depth = 0;

    struct stack_t stack;
    stack_init(&stack, depth_limit);
    stack_push(&stack, left);
    stack_push(&stack, right);

    int depth_stack[depth_limit];
    int* d_s_top = depth_stack;
    *d_s_top = 0;
    ++d_s_top;

    do {
        if (right - left >= ins_limit) {
            char *piv_r;
            char *piv_l;
            __block_partition(left, right, &piv_l, &piv_r, size, cmp);
            if (piv_l - left > right - piv_r) {
                stack_push(&stack, left);
                stack_push(&stack, piv_l);
                left = piv_l + size;
            } else {
                stack_push(&stack, piv_r + size);
                stack_push(&stack, right);
                right = piv_r + size;
            }
            depth++;
            *d_s_top = depth;
            ++d_s_top;
        } else {
            // if (right - left >= ins_limit)
            //     __heap_sort(left, right, size, cmp);
            // else
                __insertion_sort(left, right, size, cmp);
            right = stack_pop(&stack);
            left = stack_pop(&stack);
            --d_s_top;
            depth = *d_s_top;
        }
    } while (!stack_empty(&stack));

    free(stack.items);
}

inline void insertion_sort(const void *base, size_t nmemb, size_t size, compare_fn_t cmp)
{
    if (!nmemb) // Return if there are no items.
        return;

    char *base_ptr = (char *)base; // Cast the array to an array of bytes.
    char *bound_ptr = base_ptr + size * (nmemb - 1); // Create a pointer to the last byte.
    __insertion_sort(base_ptr, bound_ptr, size, cmp);    
}

inline void bubble_sort(const void *base, size_t nmemb, size_t size, compare_fn_t cmp)
{
    if (!nmemb) // Return if there are no items.
        return;
    
    char *base_ptr = (char *)base; // Cast the array to an array of bytes
    char *bound_ptr = base_ptr + size * (nmemb - 1); // Create a pointer to the last byte.
    __bubble_sort(base_ptr, bound_ptr, size, cmp);
}

inline void quicksort(const void *base, size_t nmemb, size_t size, compare_fn_t cmp) 
{
    if (!nmemb) // Return if there are no items.
        return;
    
    char *base_ptr = (char *)base; // Cast the array to an array of bytes.
    char *left_ptr = base_ptr;
    char *right_ptr = base_ptr + size * (nmemb - 1); // Create a pointer to the last byte.
    __quicksort(left_ptr, right_ptr, size, cmp);
}

inline void quicksort_rec(const void *base, size_t nmemb, size_t size, compare_fn_t cmp) 
{
    if (!nmemb) // Return if there are no items.
        return;
    
    int depth = 0;
    char *base_ptr = (char *)base; // Cast the array to an array of bytes.
    char *left_ptr = base_ptr;
    char *right_ptr = base_ptr + size * (nmemb - 1); // Create a pointer to the last byte.
    __quicksort_rec(left_ptr, right_ptr, size, cmp);
}

inline void heap_sort(const void *base, size_t nmemb, size_t size, compare_fn_t cmp)
{
    if (!nmemb) // Return if there are no items.
        return;
    
    char *base_ptr = (char *)base; // Cast the array to an array of bytes.
    char *left_ptr = base_ptr;
    char *right_ptr = base_ptr + size * (nmemb - 1); // Create a pointer to the last byte.
    __heap_sort(left_ptr, right_ptr, size, cmp);
}