#ifndef SORT_H
#define SORT_H

#include <stdbool.h>
#include <stdlib.h>

/* Defines */

#define byte_swap(a, b, size)			\
    do {								\
        size_t __size = (size);			\
        char *__a = (a), *__b = (b);	\
        do {                            \
	        char __tmp = *__a;			\
	        *__a++ = *__b;				\
	        *__b++ = __tmp;				\
	    } while (--__size > 0);			\
    } while (0)

#define byte_assert(a, b, size)         \
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

/* Generic insertion sort method */
void insertion_sort(const void *base, int left_idx, int right_idx, size_t size, __compare_fn_t cmp);

#endif