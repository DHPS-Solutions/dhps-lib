#ifndef DHPS_STRING_H
#define DHPS_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#define __STRING_DEFAULT_CAPACITY__ 32
#define CMP_ERR 1337    

/* Macro's */

/**
 * Macro for comparing two strings.
 * @param str1 The first string.
 * @param str2 The second string.
 * @return 0 if the strings are equal, 
 *         smaller than -1 if the first string is more alphabetical than the second string, 
 *         larger than 1 if the first string is less alphabetical than the second string.
 */
#define STR_CMP(str1, str2) \
({\
    register const unsigned char *__s1 = (const unsigned char *) (str1)->data;\
    register const unsigned char *__s2 = (const unsigned char *) (str2)->data;\
    register unsigned char __c1;\
    register unsigned char __c2;\
    do {\
        __c1 = *__s1++;\
        __c2 = *__s2++;\
    } while (__c1 == __c2 && __c1 != '\0');\
    __c1 - __c2;\
})

/**
 * Method to check if strings are equal.
 * @param str1 The first string.
 * @param str2 The second string.
 * @return true if the strings are equal, false if they are not.
 */
#define STR_EQ(str1, str2)\
({\
    bool __eq = true;\
    if ((str1)->length == (str2)->length) {\
        register const unsigned char *__s1 = (const unsigned char *) (str1)->data;\
        register const unsigned char *__s2 = (const unsigned char *) (str2)->data;\
        do\
            if (*__s1++ != *__s2++)\
                __eq = false;\
        while (*__s1 != '\0' && __eq);\
    } else {\
        __eq = false;\
    }\
    __eq;\
})

typedef struct string_t * string;

/**
 * Structure for storing a string.
 * @param data The string data.
 * @param length The length of the string.
 * @param capacity The capacity of the string.
 */
struct string_t {
    char *data;
    size_t length;
    size_t capacity;
};

/**
 * Creates a new string.
 * @param data The string data.
 * @return The new string.
 */
struct string_t *string_new(const char *data);

/**
 * Frees a string.
 * @param str The string to free.
 */
void string_free(struct string_t *str);

struct string_t *string_new_slice(const char *data, size_t start, size_t end);

/**
 * Gets the length of a string.
 * @param str The string.
 * @return The length of the string.
 */
size_t string_length(const struct string_t *str);

/**
 * Gets the data of a string.
 * @param str The string.
 * @return The data of the string.
 */
char *string_get(const struct string_t *str);

/**
 * Method for concatenating two strings.
 * @param src The source string.
 * @param dest The destination string.
 * @return The source string with the destination string appended.
 */
struct string_t *string_concat(struct string_t *src, const struct string_t *dest);

/**
 * Method for comparing two strings.
 * @param str1 The first string.
 * @param str2 The second string.
 * @return 0 if the strings are equal, 
 *         smaller than -1 if the first string is more alphabetical than the second string, 
 *         larger than 1 if the first string is less alphabetical than the second string.
 */
int string_compare(const struct string_t *str1, const struct string_t *str2);

/**
 * Method to check if strings are equal.
 * @param str1 The first string.
 * @param str2 The second string.
 * @return true if the strings are equal, false if they are not.
 */
bool string_equal(const struct string_t *str1, const struct string_t *str2);

int string_compare_2(const struct string_t *str1, const struct string_t *str2);

#endif
