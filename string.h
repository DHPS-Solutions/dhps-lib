#ifndef DHPS_STRING_H
#define DHPS_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#define __STRING_DEFAULT_CAPACITY__ 32
#define CMP_ERR 1337    

typedef struct string_t* string;

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
 *         larger than 1 if the source string is more alphabetical than the destination string, 
 *         smaller than -1 if the source string is less alphabetical than the destination string.
 */
int string_compare(const struct string_t *src, const struct string_t *dest);

/**
 * Method to check if strings are equal.
 * @param str1 The first string.
 * @param str2 The second string.
 * @return true if the strings are equal, false if they are not.
 */
bool string_equal(const struct string_t *str1, const struct string_t *str2);


#endif