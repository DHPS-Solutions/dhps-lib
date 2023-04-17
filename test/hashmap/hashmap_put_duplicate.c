#include "../../hashmap.h"
#include <assert.h>

int main()
{
    struct hashmap_t map;
    hashmap_init(&map);

    hashmap_ssput(&map, "key", "value", false);
    assert(map.len == 1);

    hashmap_ssput(&map, "key", "another value", false);
    assert(map.len == 1);

    hashmap_ssput(&map, "key", "another value!", true);
    assert(map.len == 1);
    
    hashmap_free(&map);
}
