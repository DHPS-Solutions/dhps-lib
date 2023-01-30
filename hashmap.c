/* Written by Nicolai H. Brand 2023 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define DHPS_BUCKET_SIZE 6
#define DHPS_STARTING_BUCKETS_log2 3
#define N_BUCKETS(log2) (2 << ((log2) - 1))

struct hashmap_entry_t {
    void *entry;
    void *key;
    size_t key_size;
    uint8_t hash_extra;         // used for faster comparison
                                // the common case is that the bytes are not equal
};

struct bucket_t {
    struct hashmap_entry_t dp[DHPS_BUCKET_SIZE];        // 6 direct items. Spatial locality bros.
    struct bucket_t *overflow;                          // NOTE: may create a special overflow
                                                        // bucket struct that does not need to be
                                                        // of size DHPS_BUCKET_SIZE
};

struct hashmap_t {
    struct bucket_t *buckets;
    uint32_t size_log2;
    size_t total_items;
};


void hashmap_init(struct hashmap_t *map)
{
    map->total_items = 0;
    map->size_log2 = DHPS_STARTING_BUCKETS_log2;

    map->buckets = malloc(sizeof(struct bucket_t) * N_BUCKETS(map->size_log2));
    struct bucket_t bucket;
    for (uint32_t i = 0; i < N_BUCKETS(map->size_log2); i++) {
        bucket = map->buckets[i];
        bucket.overflow = NULL;
        for (uint8_t i = 0; i < DHPS_BUCKET_SIZE; i++)
            bucket.dp[i].entry = NULL;
    }
}

void hashmap_free(struct hashmap_t *map)
{
    struct bucket_t bucket;
    for (uint32_t i = 0; i < N_BUCKETS(map->size_log2); i++) {
        bucket = map->buckets[i];
        if (bucket.overflow)
            free(bucket.overflow);
    }

    free(map->buckets);
}

static uint32_t hash_func(void *t, size_t ts, uint32_t size_log2)
{
    //NOTE: gigahafting kok (legger dermed ikke så mye lit til det)
    uint32_t A = 1327217885;
    uint32_t k = 0;
    for (size_t i = 0; i < ts; i++)
	k += (k << 5) + ((int8_t *)t)[i];

    return (k * A >> (31 - size_log2));
}

static inline uint8_t hash_extra(uint32_t hash)
{
    int mask = (1 << 7) - 1;
    return (uint8_t)(hash & mask);
}

static void insert_into_bucket(struct bucket_t *bucket, void *key, size_t key_size, void *t,
                               uint8_t extra)
{
    //NOTE: do we need to check if a matching entry already exists, or do we not care about that?
    //      After all, its a hashmap, not a hashset
    //      But what if we want to update an exisitng entry? remove that insert again?
    //      Just simply updating it would be nicer
    for (uint8_t i = 0; i < DHPS_BUCKET_SIZE; i++) {
        if (bucket->dp[i].entry != NULL)
            continue;

        bucket->dp[i].entry = t;
        bucket->dp[i].key = key;
        bucket->dp[i].key_size = key_size;
        bucket->dp[i].hash_extra = extra;
        return;
    }

    /* no more space in direct pointers */
    //NOTE: add overflow bucket!
}

static void *get_from_bucket(struct bucket_t *bucket, void *key, size_t key_size, uint8_t extra)
{
    for (uint8_t i = 0; i < DHPS_BUCKET_SIZE; i++) {
        if (bucket->dp[i].entry == NULL)
            continue;
        if (bucket->dp[i].hash_extra != extra || bucket->dp[i].key_size != key_size)
            continue;

        /* please compiler, do not reorder muh code ! */
        if (memcmp(bucket->dp[i].key, key, key_size) == 0) {
            return bucket->dp[i].entry;
        }
    }

    //NOTE: overflow bucket!
    return NULL;
}

void hashmap_put(struct hashmap_t *map, void *key, size_t key_size, void *t, size_t ts)
{
    uint32_t hash = hash_func(key, key_size, map->size_log2 - 1);
    uint8_t extra = hash_extra(hash);
    insert_into_bucket(&map->buckets[hash - 1], key, key_size, t, extra);
    map->total_items++;
}

void *hashmap_get(struct hashmap_t *map, void *key, size_t key_size)
{
    uint32_t hash = hash_func(key, key_size, map->size_log2 - 1);
    uint8_t extra = hash_extra(hash);
    return get_from_bucket(&map->buckets[hash - 1], key, key_size, extra);
}

bool hashmap_rm(struct hashmap_t *map);


int main()
{
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
    if (X)
        printf("%d = %d\n", x, *X);
    else
        printf("bad\n");


    hashmap_free(&map);
}
