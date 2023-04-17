#include "../../hashmap.h"
#include <assert.h>

void stress()
{
    struct hashmap_t map;
    hashmap_init(&map);

    #define N 1024

    for (int i = 0; i < N; i++)
        hashmap_put(&map, &i, 4, &i, 4, true);

    assert(map.len == N);

    for (int i = 0; i < N; i++) {
        int *p = hashmap_get(&map, &i, 4);
        assert(*p == i);
    }

    hashmap_free(&map);
}

int main()
{
    struct hashmap_t map;
    hashmap_init(&map);

    /* 
     * default hashmap has space for 48 items.
     * default hashmap will increase the total amount of buckets
     * when the load factor >= 0.75, meaning > 36 entries will 
     * add another bucket
     */

    u32 size_log2_pre = map.size_log2;
    for (int i = 0; i < 40; i++) {
        hashmap_put(&map, &i, 4, &i, 4, true);
    }

    assert(map.size_log2 == size_log2_pre + 1);
    assert(map.len == 40);

    for (int i = 0; i < 40; i++) {
        int *p = hashmap_get(&map, &i, 4);
        assert(*p == i);
    }

    hashmap_free(&map);

    stress();
}
