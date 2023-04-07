/* Written by Nicolai H. Brand (lytix.dev), 2023 */
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "../hashmap.h"

#define N 100000

int main()
{
    char *table[] = {
        "alignas",
        "alignof",
        "auto",
        "bool",
        "break",
        "case",
        "char",
        "const",
        "constexpr",
        "continue",
        "default",
        "do",
        "double",
        "else",
        "enum",
        "extern",
        "false",
        "float",
        "for",
        "goto",
        "if",
        "inline",
        "int",
        "long",
        "nullptr",
        "register",
        "restrict",
        "return",
        "short",
        "signed",
        "sizeof",
        "static",
        "static_assert",
        "struct",
        "switch",
        "thread_local",
        "true",
        "typedef",
        "typeof",
        "typeof_unqual",
        "union",
        "unsigned",
        "void",
        "volatile",
        "while",
    };

    struct hashmap_t map;
    hashmap_init(&map);
    int len = sizeof(table) / sizeof(table[0]);
    for (int i = 0; i < len; i++) {
        hashmap_sput(&map, table[i], &i, 4, true);
    }

    assert(map.len == len);

    clock_t start = clock();
    for (int i = 0; i < N; i++) {
        for (int i = 0; i < len; i++) {
            int *v = hashmap_sget(&map, table[i]);
            assert(i == *v);
        }
    }
    clock_t end = clock();

    hashmap_free(&map);

    double cpu_time_used = ((double) (end - start) * 1000) / CLOCKS_PER_SEC;
    printf("Getting %d items took %fms\n", len * N, cpu_time_used);
    printf("Time per get: %fus\n", (cpu_time_used * 1000 / (double)(len * N)));
}
