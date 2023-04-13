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
    s->data = (char*)(malloc(__CHAR_BIT__ * __STRING_DEFAULT_CAPACITY__));
    s->capacity = __STRING_DEFAULT_CAPACITY__;
    s->length = 0;

    int i = 0;

    while (*(data + i) != '\0') {
        *(s->data + i) = *(data + i);
        s->length++;
        if (i++ == s->capacity) {
            s->capacity *= 2;
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

    for (int i = 0; i < dest->length; i++)
        *(src->data + src->length++) = *(dest->data + i);

    return src;
}

inline int string_compare(const struct string_t *str1, const struct string_t *str2)
{
    register const unsigned char *s1 = (unsigned char *) str1->data;
    register const unsigned char *s2 = (unsigned char *) str2->data;
    register unsigned char c1;
    register unsigned char c2;
    do {
        c1 = *s1++;
        c2 = *s2++;
    } while (c1 == c2 && c1 != '\0');
    return c1 - c2;
}

bool string_equal(const struct string_t *str1, const struct string_t *str2)
{
    if (str1->length != str2->length)
        return false;
    
    register const unsigned char *s1 = str1->data;
    register const unsigned char *s2 = str2->data;
    
    do 
        if (*s1++ != *s2++)
            return false;
    while (*s1 != '\0');

    return true;
}
