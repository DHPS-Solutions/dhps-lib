/* Written by Nicolai H. Brand 2023 */
#ifndef HASHMAP_H
#define HASHMAP_H

//#define HASHMAP_THREAD_SAFE
#define HASHMAP_BUCKET_SIZE 6
#define HASHMAP_STARTING_OVERFLOW_BUCKET_SIZE 4
#define HASHMAP_STARTING_BUCKETS_log2 3
#define N_BUCKETS(log2) (1 << (log2))

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#ifdef HASHMAP_THREAD_SAFE
#  include <pthread.h>
#endif


struct hashmap_entry_t {
    void *value;                // if NULL then entry is marked as unused
    void *key;
    uint32_t key_size;
    uint32_t value_size;
    uint8_t hash_extra;         // used for faster comparison
                                // the common case is that the bytes are not equal.
    uint8_t alloc_flag;         // if sizeof bool and _Bool > 3 then this struct would be of size 36
                                // instead 32 due to struct padding. Therefore we use one byte to be
                                // safe we don't waste any memory space.
};

struct overflow_bucket_t {
    // NOTE: this is really just a dynamic array.
    struct hashmap_entry_t *entries;
    uint8_t size, capacity;
};

struct bucket_t {
    struct hashmap_entry_t entries[HASHMAP_BUCKET_SIZE];   // 6 direct items. Spatial locality bros.
    struct overflow_bucket_t *overflow;
};

struct hashmap_t {
    struct bucket_t *buckets;
    uint32_t size_log2;
    size_t len;         // total items stored in the hashmap
#ifdef HASHMAP_THREAD_SAFE
    pthread_mutex_t lock;
#endif
};


/* functions */
void hashmap_init(struct hashmap_t *map);

void hashmap_free(struct hashmap_t *map);

void hashmap_put_internal(struct hashmap_t *map, void *key, uint32_t key_size, void *value,
                          uint32_t value_size, bool alloc_flag);
#define hashmap_put(map, key, key_size, value, value_size) \
        hashmap_put_internal(map, key, key_size, value, value_size, false)
#define hashmap_put_alloc(map, key, key_size, value, value_size) \
        hashmap_put_internal(map, key, key_size, value, value_size, true)
#define hashmap_sput(map, key, key_size, value) \
        hashmap_put_internal(map, key, (strlen(key) + 1) * sizeof(char), key_size, value, false)
#define hashmap_ssput(map, key, key_size) \
        hashmap_put_internal(map, key, (strlen(key) + 1) * sizeof(char), key_size, (strlen(value) + 1) * sizeof(char), false)
#define hashmap_sput_alloc(map, key, key_size, value) \
        hashmap_put_internal(map, key, (strlen(key) + 1) * sizeof(char), key_size, value, true)
#define hashmap_ssput_alloc(map, key, value) \
        hashmap_put_internal(map, key, (strlen(key) + 1) * sizeof(char), value, (strlen(value) + 1) * sizeof(char), true)

void *hashmap_get(struct hashmap_t *map, void *key, size_t key_size);
#define hashmap_sget(map, key) \
        hashmap_get(map, key, (strlen(key) + 1) * sizeof(char))

bool hashmap_rm(struct hashmap_t *map, void *key, size_t key_size);
#define hashmap_srm(map, key) \
        hashmap_rm(map, key, (strlen(key) + 1) * sizeof(char))

size_t hashmap_len(struct hashmap_t *map);

#endif /* HASHMAP_H */
