/* Written by Nicolai H. Brand 2023 */
#ifndef HASHMAP_H
#define HASHMAP_H

//#define HASHMAP_THREAD_SAFE
#define HM_STARTING_BUCKETS_LOG2 3 // the amount of starting buckets
#define HM_BUCKET_SIZE 6
#define HM_OVERFLOW_SIZE 4
#define N_BUCKETS(log2) (1 << (log2))

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
//#ifdef HASHMAP_THREAD_SAFE
//#  include <pthread.h>
//#endif

#ifndef u8
#  define u8 uint8_t
#endif
#ifndef u32
#  define u32 uint32_t
#endif

/* return codes for insert() function */
#define _HM_FULL 1
#define _HM_OVERRIDE 2
#define _HM_SUCCESS 3

struct hm_entry_t {
    void *key;          // if NULL then entry is considered unused
    void *value;
    u32 key_size;
    u32 value_size;
    u8 hash_extra;      // used for faster comparison
    u8 alloc_flag;      // true if value is alloced
                        // if sizeof bool and _Bool > 3 then this struct would be of size 36
                        // instead 32 due to struct padding. Therefore we use one byte to be
                        // safe we don't waste any memory space.
};

struct hm_bucket_t {
    struct hm_entry_t entries[HM_BUCKET_SIZE];
};

struct hashmap_t {
    struct hm_bucket_t *buckets;
    u8 size_log2;
    u32 len;         // total items stored in the hashmap
//#ifdef HASHMAP_THREAD_SAFE
//    pthread_mutex_t lock;
//#endif
};

/* functions */

void hashmap_init(struct hashmap_t *map);

void hashmap_free(struct hashmap_t *map);

void hashmap_put(struct hashmap_t *map, void *key, u32 key_size, void *value, u32 val_size, bool alloc_flag);
#define hashmap_sput(map, key, value, val_size, alloc_flag) \
    hashmap_put(map, key, (strlen(key) + 1) * sizeof(char), value, val_size, alloc_flag)
#define hashmap_ssput(map, key, value, alloc_flag) \
    hashmap_put(map, key, (strlen(key) + 1) * sizeof(char), value, (strlen(value) + 1) * sizeof(char), alloc_flag)

void *hashmap_get(struct hashmap_t *map, void *key, u32 key_size);
#define hashmap_sget(map, key) \
    hashmap_get(map, key, (strlen(key) + 1) * sizeof(char))


bool hashmap_rm(struct hashmap_t *map, void *key, u32 key_size);
#define hashmap_srm(map, key) \
    hashmap_rm(map, key, (strlen(key) + 1) * sizeof(char))




#endif /* HASHMAP_H */
