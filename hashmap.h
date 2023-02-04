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

void hashmap_put(struct hashmap_t *map, void *key, size_t key_size, void *t, size_t ts);

void *hashmap_get(struct hashmap_t *map, void *key, size_t key_size);

bool hashmap_rm(struct hashmap_t *map, void *key, size_t key_size);

#endif /* HASHMAP_H */
