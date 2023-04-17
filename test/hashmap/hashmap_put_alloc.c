#include "../../hashmap.h"
#include <string.h>
#include <assert.h>

void foo(struct hashmap_t *map)
{
    hashmap_ssput(map, "key", "value", true);
}

int main()
{
    struct hashmap_t map;
    hashmap_init(&map);

    foo(&map);

    void *v = hashmap_sget(&map, "key");

    assert(strcmp((char *)v, "value") == 0);
    hashmap_free(&map);
}
