#ifndef SORT_H
#define SORT_H

#include <stdbool.h>
#include <stdlib.h>

/* Defines */

/* 
    Macro to that finds the memory address in the middle. 
    @param high high memory address.
    @param low low memory address.
    @param size size of each element.
*/
#define FIND_MID(high, low, size) (size * ((high - left) / size >> 1))

/* Defined value for amount of before switching to insertion sort. */
#define RUN_INSERTION 50

/* 
    Macro to that returns the smallest of 2 items. 
    @param a number value.
    @param b number value.
*/
#define min(a, b) ((a) < (b) ? (a) : (b))

/* The amount of elements per block. */
#define BLOCK 256

/* 
    Macro that swaps value a and value b. Grabbed from the glibc.
    @param a char*: byte sequence a.
    @param b char*: byte sequence b.
    @param size size_t: amount of bytes per sequence.
*/
#define BYTE_SWAP(a, b, size)           \
    do {                                \
        size_t __size = (size);         \
        char *__a = (a);                \
        char *__b = (b);                \
        do {                            \
	        char __tmp = *__a;          \
	        *__a++ = *__b;              \
	        *__b++ = __tmp;             \
	    } while (--__size > 0);         \
    } while (0)

/* 
    Macro that asserts a to value b.
    @param a char*: byte sequence a.
    @param b char*: byte sequence b.
    @param size size_t: amount of bytes per sequence.
*/
#define BYTE_ASSERT(a, b, size)         \
    do {                                \
        size_t __size = (size);         \
        char *__a = (a);                \
        char *__b = (b);                \
        do {                            \
            *__a++ = *__b++;            \
        } while(--__size > 0);          \
    } while(0)               
    
/* Methods */

/* 
    Function used to assert a generic to a new value. 
    @param a const void*: a value to compare.
    @param b const void*: b value to compare.
*/
typedef bool compare_fn_t(const void *a, const void *b);

/* 
    Generic insertion sort method. 
    @param base const void*: the items that are to be sorted.
    @param nmemb size_t: the number of items (members).
    @param size size_t: the size of each item (member).
    @param cmp compare_fn_t: the comparison function to compare items (members).
*/
void insertion_sort(const void *base, size_t nmemb, size_t size, compare_fn_t cmp);

/* 
    Generic bubble sort method. 
    @param base const void*: the items that are to be sorted.
    @param nmemb size_t: the number of items (members).
    @param size size_t: the size of each item (member).
    @param cmp compare_fn_t: the comparison function to compare items (members).
*/
void bubble_sort(const void *base, size_t nmemb, size_t size, compare_fn_t cmp);

/* 
    Generic quicksort method. 
    @param base const void*: the items that are to be sorted.
    @param nmemb size_t: the number of items (members).
    @param size size_t: the size of each item (member).
    @param cmp compare_fn_t: the comparison function to compare items (members).
*/
void quicksort(const void *base, size_t nmemb, size_t size, compare_fn_t cmp) ;

#endif