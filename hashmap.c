/* Written by Nicolai H. Brand 2023 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#define DHPS_BUCKET_SIZE 6
#define DHPS_STARTING_OVERFLOW_BUCKET_SIZE 8
#define DHPS_STARTING_BUCKETS_log2 3
#define N_BUCKETS(log2) (2 << ((log2) - 1))

struct hashmap_entry_t {
    void *value;
    void *key;
    size_t key_size;
    uint8_t hash_extra;         // used for faster comparison
                                // the common case is that the bytes are not equal
};

struct overflow_bucket_t {
    // NOTE: this is really just a dynamic array.
    struct hashmap_entry_t *entries;
    uint8_t size, capacity;
};

struct bucket_t {
    struct hashmap_entry_t entries[DHPS_BUCKET_SIZE];   // 6 direct items. Spatial locality bros.
    struct overflow_bucket_t *overflow;
};

struct hashmap_t {
    struct bucket_t *buckets;
    uint32_t size_log2;
    size_t len;         // total items stored in the hashmap
};


void hashmap_init(struct hashmap_t *map)
{
    map->len = 0;
    map->size_log2 = DHPS_STARTING_BUCKETS_log2;

    map->buckets = malloc(sizeof(struct bucket_t) * N_BUCKETS(map->size_log2));
    struct bucket_t bucket;
    for (uint32_t i = 0; i < N_BUCKETS(map->size_log2); i++) {
        bucket = map->buckets[i];
        bucket.overflow = NULL;
        for (uint8_t i = 0; i < DHPS_BUCKET_SIZE; i++)
            bucket.entries[i].value = NULL;
    }
}

void hashmap_free(struct hashmap_t *map)
{
    struct bucket_t bucket;
    for (uint32_t i = 0; i < N_BUCKETS(map->size_log2); i++) {
        bucket = map->buckets[i];
        if (bucket.overflow != NULL) {
            free(bucket.overflow->entries);
            free(bucket.overflow);
        }
    }

    free(map->buckets);
}

static uint32_t hash_func(void *t, size_t ts)
{
    //NOTE: gigahafting kok (legger dermed ikke så mye lit til det)
    uint32_t A = 1327217885;
    uint32_t k = 0;
    for (size_t i = 0; i < ts; i++)
	k += (k << 5) + ((int8_t *)t)[i];

    return k * A;
    //return (k * A >> (32 - size_log2));
}

static inline uint8_t hash_extra(uint32_t hash)
{
    int mask = (1 << 8) - 1;
    return (uint8_t)(hash & mask);
}

static struct overflow_bucket_t *overflow_alloc()
{
    struct overflow_bucket_t *overflow = malloc(sizeof(struct overflow_bucket_t));
    overflow->entries = malloc(sizeof(struct hashmap_entry_t) * DHPS_STARTING_OVERFLOW_BUCKET_SIZE);
    overflow->size = 0;
    overflow->capacity = DHPS_STARTING_OVERFLOW_BUCKET_SIZE;
    return overflow;
}

static void insert_into_overflow(struct overflow_bucket_t *overflow, void *key, size_t key_size,
                                 void *value, uint8_t extra)
{
    /* this should really never happen assuming the hash function spreads values evenly */
    if (overflow->size >= overflow->capacity) {
        printf("nooooooooo %d\n", overflow->size);
        exit(1);
    }

    struct hashmap_entry_t *found = &overflow->entries[overflow->size++];
    found->value = value;
    found->key = key;
    found->key_size = key_size;
    found->hash_extra = extra;
}

static void insert_into_bucket(struct bucket_t *bucket, void *key, size_t key_size, void *value,
                               uint8_t extra)
{
    //NOTE: do we need to check if a matching entry already exists, or do we not care about that?
    //      After all, its a hashmap, not a hashset
    //      But what if we want to update an exisitng entry? remove that insert again?
    //      Just simply updating it would be nicer
    for (uint8_t i = 0; i < DHPS_BUCKET_SIZE; i++) {
        if (bucket->entries[i].value != NULL)
            continue;

        bucket->entries[i].value = value;
        bucket->entries[i].key = key;
        bucket->entries[i].key_size = key_size;
        bucket->entries[i].hash_extra = extra;
        return;
    }

    /* no more space in direct pointers */
    if (bucket->overflow == NULL)
        bucket->overflow = overflow_alloc();

    insert_into_overflow(bucket->overflow, key, key_size, value, extra);
}

static void *get_from_bucket(struct bucket_t *bucket, void *key, size_t key_size, uint8_t extra)
{
    for (uint8_t i = 0; i < DHPS_BUCKET_SIZE; i++) {
        if (bucket->entries[i].value == NULL)
            continue;
        if (bucket->entries[i].hash_extra != extra || bucket->entries[i].key_size != key_size)
            continue;

        /* please compiler, do not reorder muh code ! */
        if (memcmp(bucket->entries[i].key, key, key_size) == 0)
            return bucket->entries[i].value;
    }

    /* value not found, check overflow bucket */
    if (bucket->overflow == NULL)
        return NULL;

    struct overflow_bucket_t *overflow = bucket->overflow;
    for (uint8_t i = 0; i < overflow->size; i++) {
        if (overflow->entries[i].value == NULL)
            continue;
        if (overflow->entries[i].hash_extra != extra || overflow->entries[i].key_size != key_size)
            continue;

        if (memcmp(overflow->entries[i].key, key, key_size) == 0)
            return overflow->entries[i].value;
    }

    return NULL;
}

static void move_entry(struct bucket_t *new_buckets, uint32_t size_log2, struct hashmap_entry_t *entry)
{
    uint32_t hash_full = hash_func(entry->key, entry->key_size);
    uint32_t hash = hash_full >> (32 - size_log2);
    uint8_t extra = hash_extra(hash_full);
    insert_into_bucket(&new_buckets[hash], entry->key, entry->key_size, entry->value, extra);
}

static void move_entries(struct hashmap_t *map, struct bucket_t *new_buckets)
{
    // why -1? because thats the old size
    for (uint32_t i = 0; i < N_BUCKETS(map->size_log2 - 1); i++) {
        struct bucket_t old_bucket = map->buckets[i];
        for (uint8_t j = 0; j < DHPS_BUCKET_SIZE; j++) {
            if (old_bucket.entries[j].value != NULL)
                move_entry(new_buckets, map->size_log2, &old_bucket.entries[j]);
        }

        /* move all overflow entries as well */
        if (old_bucket.overflow != NULL) {
            for (uint8_t j = 0; j < old_bucket.overflow->size; j++) {
                if (old_bucket.overflow->entries[j].value != NULL)
                    move_entry(new_buckets, map->size_log2, &old_bucket.overflow->entries[j]);
            }
        }
    }

    free(map->buckets);
    map->buckets = new_buckets;
}

void hashmap_put(struct hashmap_t *map, void *key, size_t key_size, void *t, size_t ts)
{
    double load_factor = (double)map->len / (N_BUCKETS(map->size_log2) * DHPS_BUCKET_SIZE);
    if (load_factor >= 0.75) {
        // NOTE: instead of moving all entries in one go, we can only move over some buckets
        //       and invalidate the rest and move them once necesary
        /* 
         * just reallocating the buckets is not really feasable since every entry has to be moved,
         * to its new bucket. So if the buckets are already filled with old invalid entries, the new
         * entries would not have any space.
         */
        map->size_log2++;
        assert(map->size_log2 < 32);
        struct bucket_t *new_buckets = malloc(sizeof(struct bucket_t) * N_BUCKETS(map->size_log2));
        for (uint32_t i = 0; i < N_BUCKETS(map->size_log2); i++) {
            struct bucket_t bucket = new_buckets[i];
            bucket.overflow = NULL;
            for (uint8_t i = 0; i < DHPS_BUCKET_SIZE; i++)
                bucket.entries[i].value = NULL;
        }

        move_entries(map, new_buckets);
    }

    uint32_t hash_full = hash_func(key, key_size);
    uint32_t hash = hash_full >> (32 - map->size_log2);
    uint8_t extra = hash_extra(hash_full);
    //printf("alpha: %f h: %d len: %zu size: %d\n", load_factor, hash, map->len, map->size_log2);
    insert_into_bucket(&map->buckets[hash], key, key_size, t, extra);
    map->len++;
}

void *hashmap_get(struct hashmap_t *map, void *key, size_t key_size)
{
    uint32_t hash_full = hash_func(key, key_size);
    uint32_t hash = hash_full >> (32 - map->size_log2);
    uint8_t extra = hash_extra(hash_full);
    return get_from_bucket(&map->buckets[hash], key, key_size, extra);
}

bool hashmap_rm(struct hashmap_t *map);


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
