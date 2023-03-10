#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../hashmap.h"


void stress_test()
{
    struct hashmap_t map;
    hashmap_init(&map);

    #define N 1024

    char key[128];
    for (int i = 0; i < N; i++) {
        snprintf(key, 32, "hello:%d!", i);
        char *cpy = malloc(32);
        strcpy(cpy, key);
        hashmap_sput_alloc(&map, cpy, &i, 4);
    }

    for (int i = 0; i < N; i++) {
        snprintf(key, 32, "hello:%d!", i);
        int *x = hashmap_sget(&map, key);
        assert(*x == i);

        if (i % 11 == 0)
            hashmap_srm(&map, key);
    }
}

void foo(struct hashmap_t *map)
{
    /* values are copied over to the heap */
    hashmap_ssput_alloc(map, "key1", "value1");
    hashmap_ssput_alloc(map, "key2", "value2");
}

void deep_copy_test()
{
    struct hashmap_t map;
    hashmap_init(&map);
    foo(&map);
    assert(strcmp(hashmap_sget(&map, "key1"), "value1") == 0);
    assert(strcmp(hashmap_sget(&map, "key2"), "value2") == 0);
    hashmap_free(&map);
}

void simple()
{
    struct hashmap_t map;
    hashmap_init(&map);
    int x = 1;
    hashmap_sput_alloc(&map, "yo", &x, sizeof(int));
    hashmap_free(&map);
}

int main()
{
    /* puts and later gets a bunch of entries in a hashmap */
    simple();
    return 0;
    stress_test();
    deep_copy_test();

    struct hashmap_t map;
    hashmap_init(&map);

    int x = 1;
    int y = 2;
    int z = 3;

    char *a = "hello";
    hashmap_put(&map, a, strlen(a) + 1, &x, 1);
    char *b = "hellO";
    hashmap_sput(&map, b, &y, 1);
    char *c = "hello.";
    hashmap_sput(&map, c, &z, 1);

    int *X = hashmap_get(&map, a, strlen(a) + 1);
    int *Y = hashmap_get(&map, b, strlen(b) + 1);
    int *Z = hashmap_sget(&map, c);
    assert(*X == x && *Y == y && *Z == z);

    assert(hashmap_len(&map) == 3);
    bool rc;
    /* removing an item */
    rc = hashmap_rm(&map, a, strlen(a) + 1);
    assert(rc == true);
    assert(hashmap_len(&map) == 2);

    /* removing an item that is already removed */
    rc = hashmap_rm(&map, a, strlen(a) + 1);
    assert(rc == false);
    X = hashmap_sget(&map, a);
    assert(X == NULL);
    assert(hashmap_len(&map) == 2);

    hashmap_free(&map);
}
