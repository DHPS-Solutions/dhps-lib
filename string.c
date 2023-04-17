#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "common.h"
#include "string.h"

struct string_t *string_new(const char *data)
{
    if (data == NULL)
        return NULL;
    
    struct string_t *s = malloc(sizeof(struct string_t));
    s->data = calloc(__STRING_DEFAULT_CAPACITY__, __CHAR_BIT__);
    s->capacity = __STRING_DEFAULT_CAPACITY__;
    s->length = 0;

    size_t i = 0;
    while (*(data + i) != '\0') {
        *(s->data + i) = *(data + i);
        s->length++;
        if (i++ == s->capacity) {
            s->capacity += __STRING_DEFAULT_CAPACITY__;
            s->data = (char *)(realloc(s->data, __CHAR_BIT__ * s->capacity));
        }
    }

    return s;
}

void string_free(struct string_t *s)
{
    if (s == NULL)
        return;

    free(s->data);
    free(s);
}

struct string_t *string_new_slice(const char *data, size_t start, size_t end)
{
    /* why +2? 
     * 1. Null termination.
     * 2. We the entire data from the start position to the end position to be present.
     *    say start = 0 and end = 1, we want both the 0th char and the 1st char, i.e end - start + 1.
     */
    size_t len = end - start + 2;
    struct string_t *s = malloc(sizeof(struct string_t));
    s->capacity = __STRING_DEFAULT_CAPACITY__ >= len ? __STRING_DEFAULT_CAPACITY__ : len;
    s->data = (char *)(malloc(__CHAR_BIT__ * s->capacity));
    s->length = len;

    memcpy(s->data, data + start, len);
    s->data[s->length - 1] = 0;
    return s;
}

size_t string_length(const struct string_t *s)
{
    if (s == NULL)
        return 0;

    return s->length;
}

char *string_get(const struct string_t *s)
{
    if (s == NULL)
        return NULL;

    return s->data;
}

struct string_t *string_concat(struct string_t *src, const struct string_t *dest)
{
    if (src == NULL || dest == NULL)
        return NULL;
    
    while (src->capacity < src->length + dest->length)
        src->capacity += __STRING_DEFAULT_CAPACITY__;

    for (size_t i = 0; i < dest->length; i++)
        *(src->data + src->length++) = *(dest->data + i);

    return src;
}

inline int string_compare(const struct string_t *str1, const struct string_t *str2)
{
    register const unsigned char *__s1 = (const unsigned char *) str1->data;
    register const unsigned char *__s2 = (const unsigned char *) str2->data;
    
    register unsigned char __c1;
    register unsigned char __c2;

    do {
        __c1 = *__s1++;
        __c2 = *__s2++;
        if (__c1 == '\0')
            return __c1 - __c2;
    } while (__c1 == __c2);

    return __c1 - __c2;
}

inline bool string_equal(const struct string_t *str1, const struct string_t *str2)
{
    if (str1->length != str2->length)
        return false;
    
    register const unsigned char *s1 = (unsigned char *) str1->data;
    register const unsigned char *s2 = (unsigned char *) str2->data;
    
    do 
        if (*s1++ != *s2++)
            return false;
    while (*s1 != '\0');

    return true;
}
