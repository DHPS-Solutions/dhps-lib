/* Written by Nicolai H. Brand 2023 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define DHPS_BUCKET_SIZE 6
#define DHPS_STARTING_BUCKETS 8

struct hashmap_entry_t {
    void *entry;
    size_t size;
    uint8_t hash_extra_and_alloc_flag;          // used for faster comparison
                                                // the common case is that the bytes are not equal
};

struct bucket_t {
    struct hashmap_entry_t dp[DHPS_BUCKET_SIZE];         // 6 direct pointers to items
    struct bucket_t *overflow;
};

struct hashmap_t {
    struct bucket_t *buckets;
    uint32_t bucket_size;
    size_t total_items;
};



void hashmap_init(struct hashmap_t *map)
{
    map->total_items = 0;
    map->bucket_size = DHPS_STARTING_BUCKETS;

    map->buckets = malloc(sizeof(struct bucket_t) * map->bucket_size);
    struct bucket_t bucket;
    //for (uint32_t i = 0; i < map->bucket_size; i++) {
    //    bucket = map->buckets[i];
    //    for (int j = 0; j < DHPS_BUCKET_SIZE; j++)
    //        bucket.dp[j] = NULL;
    //    bucket.overflow = NULL;
    //}
}

void hashmap_free(struct hashmap_t *map)
{
    struct bucket_t bucket;
    for (uint32_t i = 0; i < map->bucket_size; i++) {
        bucket = map->buckets[i];
        if (bucket.overflow)
            free(bucket.overflow);
    }

    free(map->buckets);
}

void hashmap_put(struct hashmap_t *map, void *t, size_t ts);
void *hashmap_get(struct hashmap_t *map);
bool hashmap_rm(struct hashmap_t *map);


int main()
{
    struct hashmap_t map;
    hashmap_init(&map);
    hashmap_free(&map);
}
