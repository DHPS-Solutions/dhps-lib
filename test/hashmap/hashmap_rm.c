#include "../../hashmap.h"
#include <assert.h>

int main()
{
    struct hashmap_t map;
    hashmap_init(&map);

    hashmap_ssput(&map, "key", "value", false);
    assert(map.len == 1);

    hashmap_srm(&map, "key");
    assert(map.len == 0);

    // with heap allocation
    hashmap_ssput(&map, "key2", "value", true);
    assert(map.len == 1);

    hashmap_srm(&map, "key2");
    assert(map.len == 0);
    
    hashmap_free(&map);
}
