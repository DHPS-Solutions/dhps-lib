#ifndef SORT_H
#define SORT_H

#include <stdbool.h>
#include <stdlib.h>

/* Defines */
#define FIND_MID(high, low, size) (size * ((high - left) / size >> 1))

#define RUN_INSERTION(size) (100 * size)

/* Macro that swaps value a and value b. Grabbed from the glibc. */
#define BYTE_SWAP(a, b, size)			\
    do {								\
        size_t __size = (size);			\
        char *__a = (a), *__b = (b);	\
        do {                            \
	        char __tmp = *__a;			\
	        *__a++ = *__b;				\
	        *__b++ = __tmp;				\
	    } while (--__size > 0);			\
    } while (0)

/* Macro that asserts a to value b. */
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

/* Function used to assert a generic to a new value. */
typedef bool compare_fn_t(const void *, const void *);

/* Generic insertion sort method. */
void insertion_sort(const void *base, size_t nmemb, size_t size, compare_fn_t cmp);

/* Generic quicksort method. */
void quicksort(const void* base, size_t nmemb, size_t size, compare_fn_t cmp) ;

#endif