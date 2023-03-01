/*
    Collection of common functions, macros and definitions.

    By Callum Gran and Nicolai Brand Hollup 2023.
*/

#ifndef COMMON_H
#define COMMON_H

/* Includes */
#include <limits.h>
#include <stdint.h>
#ifdef DYNAMIC_MACROS
    #include <stdlib.h>
#endif

/* Maths */

/* Method's to find minimum. */
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#ifdef LESS_BRANCHING
    #undef MIN
    #define MIN(a, b) (b ^ ((a ^ b) & -(a < b)))
#endif

/* Method's to find maximum. */
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#ifdef LESS_BRANCHING
    #undef MAX
    #define MAX(a, b) (a ^ ((a ^ b) & -(a < b)))
#endif

/* Method to find absolute value. */
#define ABS(a) ((a) < 0 ? -(a) : (a))

/* Method to get absolute value of integer without branching. */

#ifdef LESS_BRANCHING
    #undef ABS
    #define ABS_INT_MASK(a) (v >> sizeof(int32_t) * CHAR_BIT - 1)
    #define ABS_INT(a) ((a + ABS_INT_MASK(a)) ^ ABS_INT_MASK(a))
#endif

/* Method to find i an integer is a power of 2. */
#define IS_POWER_OF_2(i) ((i) && !((i) & ((i) - 1)))


/* Byte sequence macros. */

/* 
    Macro that swaps value a and value b. Grabbed from the glibc.
    @param a char*: byte sequence a.
    @param b char*: byte sequence b.
    @param size size_t: amount of bytes per sequence.
*/
#define BYTE_SWAP(a, b, size)                       \
    do {                                            \
        size_t __size = (size);                     \
        register unsigned char *__a = (a);          \
        register unsigned char *__b = (b);          \
        do {                                        \
	        register unsigned char __tmp = *__a;    \
	        *__a++ = *__b;                          \
	        *__b++ = __tmp;                         \
	    } while (--__size > 0);                     \
    } while (0)

/*
    Macro that rotates a byte sequence.
    @param a char*: byte sequence a.
    @param b char*: byte sequence b.
    @param c char*: byte sequence c.
    @param size size_t: amount of bytes per sequence.
*/
#define THREE_BYTE_ROTATE(a, b, c, size)            \
    do {                                            \
        size_t __size = (size);                     \
        register unsigned char *__a = (a);          \
        register unsigned char *__b = (b);          \
        register unsigned char *__c = (c);          \
        do {                                        \
	        register unsigned char __tmp = *__a;    \
	        *__a++ = *__b;                          \
	        *__b++ = *__c;                          \
            *__c++ = __tmp;                         \
	    } while (--__size > 0);                     \
    } while (0)

/* 
    Macro that asserts a to value b.
    @param a char*: byte sequence a.
    @param b char*: byte sequence b.
    @param size size_t: amount of bytes per sequence.
*/
#define BYTE_ASSERT(a, b, size)                     \
    do {                                            \
        size_t __size = (size);                     \
        register unsigned char *__a = (a);          \
        register const unsigned char *__b = (b);    \
        do {                                        \
            *__a++ = *__b++;                        \
        } while(--__size > 0);                      \
    } while(0)

/*
    Macro that checks if a and b are equal.
    @param a char*: byte sequence a.
    @param b char*: byte sequence b.
    @param size size_t: amount of bytes per sequence.
    @param pred bool: predicate to check if a and b are equal.
*/
#define BYTES_EQUAL(a, b, size, pred)               \
    do {                                            \
        size_t __size = (size);                     \
        register const unsigned char *__a = (a);    \
        register const unsigned char *__b = (b);    \
        do {                                        \
            pred = (*__a++ == *__b++);              \
        } while(pred && --__size != 0);             \
    } while(0)                          

#ifdef DYNAMIC_MACROS
    /* Dynamic Methods */

    /*
        Macro that ensures that the size of a dynamic object is not exceeded.
        @param size size_t: current size of the object.
        @param cap size_t: current capacity of the object.
        @param vals void**: pointer to the object.
    */
    #define ENSURE_CAP(size, cap, vals)                     \
        do {                                                \
            if (size == cap) {                              \
                cap <<= 1;                                  \
                vals = (void **)                            \
                    (realloc(vals, cap * sizeof(void *)));  \
            }                                               \
        } while(0)

    /*
        Macro that reduces the size of a dynamic object.
        @param size size_t: current size of the object.
        @param cap size_t: current capacity of the object.
        @param vals void**: pointer to the object.
    */
    #define REDUCE_CAP(size, cap, vals)                     \
        do {                                                \
            if (size == (cap >> 1)) {                       \
                cap >>= 1;                                  \
                vals = (void **)                            \
                    (realloc(vals, cap * sizeof(void *)));  \
            }                                               \
        } while(0)
                                            
#endif

#endif