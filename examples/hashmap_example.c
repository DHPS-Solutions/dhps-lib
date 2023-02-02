#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../hashmap.h"

void stress_test()
{
    struct hashmap_t map;
    hashmap_init(&map);

    #define N 1000

    char key[128];
    for (int i = 0; i < N; i++) {
        snprintf(key, 32, "hello:%d!", i);
        char *cpy = malloc(32);
        strcpy(cpy, key);
        int *cpy_i = malloc(4);
        *cpy_i = i;
        hashmap_put(&map, cpy, strlen(cpy) + 1, cpy_i, 4);
    }

    for (int i = 0; i < N; i++) {
        snprintf(key, 32, "hello:%d!", i);
        int *x = hashmap_get(&map, key, strlen(key) + 1);
        assert(*x == i);
    }
}

int main()
{
    stress_test();

    struct hashmap_t map;
    hashmap_init(&map);

    int x = 1;
    int y = 2;
    int z = 3;

    char *a = "hello";
    hashmap_put(&map, a, strlen(a) + 1, &x, 1);
    char *b = "hellO";
    hashmap_put(&map, b, strlen(b) + 1, &y, 1);
    char *c = "hello.";
    hashmap_put(&map, c, strlen(c) + 1, &z, 1);

    int *X = hashmap_get(&map, a, strlen(a) + 1);
    int *Y = hashmap_get(&map, b, strlen(b) + 1);
    int *Z = hashmap_get(&map, c, strlen(c) + 1);
    assert(*X == x && *Y == y && *Z == z);

    hashmap_free(&map);
}
