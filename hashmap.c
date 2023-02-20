/* Written by Nicolai H. Brand 2023 */
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#ifdef HASHMAP_THREAD_SAFE
#  include <pthread.h>
#endif

#include "hashmap.h"


static uint32_t hash_func(void *t, uint32_t ts)
{
    //NOTE: gigahafting kok (legger dermed ikke så mye lit til det)
    uint32_t A = 1327217885;
    uint32_t k = 0;
    for (uint32_t i = 0; i < ts; i++)
	k += (k << 5) + ((int8_t *)t)[i];

    return k * A;
}

static inline uint8_t hash_extra(uint32_t hash)
{
    int mask = (1 << 8) - 1;
    return (uint8_t)(hash & mask);
}

static struct overflow_bucket_t *overflow_alloc()
{
    struct overflow_bucket_t *overflow = malloc(sizeof(struct overflow_bucket_t));
    overflow->entries = malloc(sizeof(struct hashmap_entry_t) * HASHMAP_STARTING_OVERFLOW_BUCKET_SIZE);
    overflow->size = 0;
    overflow->capacity = HASHMAP_STARTING_OVERFLOW_BUCKET_SIZE;
    return overflow;
}

static struct hashmap_entry_t *scan_overflow_for_match(struct overflow_bucket_t *overflow,
                                                       void *key, uint32_t key_size, uint8_t extra)
{
    for (uint8_t i = 0; i < overflow->size; i++) {
        if (overflow->entries[i].value == NULL)
            continue;
        if (overflow->entries[i].hash_extra != extra || overflow->entries[i].key_size != key_size)
            continue;

        if (memcmp(overflow->entries[i].key, key, key_size) == 0)
            return &overflow->entries[i];
    }

    return NULL;
}

static inline void insert_entry(struct hashmap_entry_t *entry, void *key, uint32_t key_size,
                                void *value, uint32_t value_size, uint8_t extra, bool alloc_flag)
{
    entry->key = key;
    entry->key_size = key_size;
    if (alloc_flag) {
        entry->value = malloc(value_size);
        memcpy(entry->value, value, value_size);
    } else {
        entry->value = value;
    }
    entry->value_size = value_size;
    entry->hash_extra = extra;
    entry->alloc_flag = (uint8_t)alloc_flag;
}

static void insert_into_overflow(struct overflow_bucket_t *overflow, void *key, uint32_t key_size,
                                 void *value, uint32_t value_size, uint8_t extra, bool alloc_flag)
{
    /* this should really never happen assuming the hash function spreads values evenly */
    if (overflow->size >= overflow->capacity) {
        //TODO: resize of overflow bucket or force resize entire map?
        exit(1);
    }

    struct hashmap_entry_t *found = &overflow->entries[overflow->size++];
    insert_entry(found, key, key_size, value, value_size, extra, alloc_flag);
}

static bool insert_into_bucket(struct bucket_t *bucket, void *key, uint32_t key_size, void *value,
                               uint32_t value_size, uint8_t extra, bool alloc_flag)
{
    /*
     * Our hashmap implementation does not allow duplcate entries.
     * Therefore, we cannot simply find the first empty entry and set the new entry here, we have 
     * to make sure the entry we are inserting does not already exist somewhere else in the bucket.
     * If we find a matching entry, we simply override. If we do not find a matching entry, we
     * insert the new entry in the first found empty entry.
     */
    struct hashmap_entry_t *found = NULL;

    for (uint8_t i = 0; i < HASHMAP_BUCKET_SIZE; i++) {
        if (bucket->entries[i].value != NULL) {
            /* check if stored entry has the same key as entry we want to store */
            if (bucket->entries[i].hash_extra != extra || bucket->entries[i].key_size != key_size)
                continue;

            if (memcmp(bucket->entries[i].key, key, key_size) == 0) {
                insert_entry(&bucket->entries[i], key, key_size, value, value_size, extra,
                             alloc_flag);
                return false;
            } else {
                continue;
            }
        }

        if (found == NULL)
            found = &bucket->entries[i];
    }

    if (found == NULL) {
        if (bucket->overflow == NULL)
            bucket->overflow = overflow_alloc();
        insert_into_overflow(bucket->overflow, key, key_size, value, value_size, extra, alloc_flag);
    } else {
        /* check overflow bucket for duplicate entry */
        if (bucket->overflow != NULL) {
            struct hashmap_entry_t *duplicate = scan_overflow_for_match(bucket->overflow, key,
                                                                        key_size, extra);
            insert_entry(duplicate != NULL ? duplicate : found, key, key_size, value, value_size,
                         extra, alloc_flag);
        } else {
            insert_entry(found, key, key_size, value, value_size, extra, alloc_flag);
        }
    }

    return true;
}

static struct hashmap_entry_t *get_from_bucket(struct bucket_t *bucket, void *key,
                                               uint32_t key_size, uint8_t extra)
{
    for (uint8_t i = 0; i < HASHMAP_BUCKET_SIZE; i++) {
        if (bucket->entries[i].value == NULL)
            continue;
        if (bucket->entries[i].hash_extra != extra || bucket->entries[i].key_size != key_size)
            continue;

        /* please compiler, do not reorder muh code ! */
        if (memcmp(bucket->entries[i].key, key, key_size) == 0)
            return &bucket->entries[i];
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
            return &overflow->entries[i];
    }

    return NULL;
}

static void hash_and_insert(struct bucket_t *new_buckets, uint32_t size_log2,
                            struct hashmap_entry_t *entry)
{
    uint32_t hash_full = hash_func(entry->key, entry->key_size);
    uint32_t hash = hash_full >> (32 - size_log2);
    uint8_t extra = hash_extra(hash_full);
    insert_into_bucket(&new_buckets[hash], entry->key, entry->key_size, entry->value,
                       entry->value_size, extra, entry->alloc_flag);
}

static void move_entries(struct hashmap_t *map, struct bucket_t *new_buckets)
{
    // why -1? because thats the old size
    for (uint32_t i = 0; i < N_BUCKETS(map->size_log2 - 1); i++) {
        struct bucket_t old_bucket = map->buckets[i];
        for (uint8_t j = 0; j < HASHMAP_BUCKET_SIZE; j++) {
            if (old_bucket.entries[j].value != NULL)
                hash_and_insert(new_buckets, map->size_log2, &old_bucket.entries[j]);
        }

        /* move all overflow entries as well */
        if (old_bucket.overflow != NULL) {
            for (uint8_t j = 0; j < old_bucket.overflow->size; j++) {
                if (old_bucket.overflow->entries[j].value != NULL)
                    hash_and_insert(new_buckets, map->size_log2, &old_bucket.overflow->entries[j]);
            }
        }
    }

    //TODO: this is not sufficient
    free(map->buckets);
    map->buckets = new_buckets;
}

void hashmap_init(struct hashmap_t *map)
{
#ifdef HASHMAP_THREAD_SAFE
    pthread_mutex_init(&map->lock, NULL);
#endif
    map->len = 0;
    map->size_log2 = HASHMAP_STARTING_BUCKETS_log2;

    map->buckets = malloc(sizeof(struct bucket_t) * N_BUCKETS(map->size_log2));
    struct bucket_t bucket;
    for (uint32_t i = 0; i < N_BUCKETS(map->size_log2); i++) {
        bucket = map->buckets[i];
        bucket.overflow = NULL;
        for (uint8_t i = 0; i < HASHMAP_BUCKET_SIZE; i++)
            bucket.entries[i].value = NULL;
    }
}

void hashmap_free(struct hashmap_t *map)
{
#ifdef HASHMAP_THREAD_SAFE
    pthread_mutex_lock(&map->lock);
#endif
    struct bucket_t bucket;
    for (uint32_t i = 0; i < N_BUCKETS(map->size_log2); i++) {
        bucket = map->buckets[i];
        for (int j = 0; i < HASHMAP_BUCKET_SIZE; i++) {
            if (bucket.entries[j].alloc_flag)
                free(bucket.entries[j].value);
        }

        if (bucket.overflow != NULL) {
            free(bucket.overflow->entries);
            free(bucket.overflow);
        }
    }

    free(map->buckets);
#ifdef HASHMAP_THREAD_SAFE
    pthread_mutex_unlock(&map->lock);
#endif
}

void hashmap_put_internal(struct hashmap_t *map, void *key, uint32_t key_size, void *value,
                          uint32_t value_size, bool alloc_flag)
{
#ifdef HASHMAP_THREAD_SAFE
    pthread_mutex_lock(&map->lock);
#endif
    double load_factor = (double)map->len / (N_BUCKETS(map->size_log2) * HASHMAP_BUCKET_SIZE);
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
            for (uint8_t i = 0; i < HASHMAP_BUCKET_SIZE; i++)
                bucket.entries[i].value = NULL;
        }

        move_entries(map, new_buckets);
    }

    uint32_t hash_full = hash_func(key, key_size);
    uint32_t hash = hash_full >> (32 - map->size_log2);
    uint8_t extra = hash_extra(hash_full);
    bool rc = insert_into_bucket(&map->buckets[hash], key, key_size, value, value_size, extra, alloc_flag);
    if (rc)
        map->len++;

#ifdef HASHMAP_THREAD_SAFE
    pthread_mutex_unlock(&map->lock);
#endif
}

void *hashmap_get(struct hashmap_t *map, void *key, uint32_t key_size)
{
#ifdef HASHMAP_THREAD_SAFE
    pthread_mutex_lock(&map->lock);
#endif
    uint32_t hash_full = hash_func(key, key_size);
    uint32_t hash = hash_full >> (32 - map->size_log2);
    uint8_t extra = hash_extra(hash_full);
#ifdef HASHMAP_THREAD_SAFE
    pthread_mutex_unlock(&map->lock);
#endif
    struct hashmap_entry_t *found = get_from_bucket(&map->buckets[hash], key, key_size, extra);
    if (found == NULL)
        return NULL;
    return found->value;
}

bool hashmap_rm(struct hashmap_t *map, void *key, uint32_t key_size)
{
    if (map->len == 0)
        return false;

#ifdef HASHMAP_THREAD_SAFE
    pthread_mutex_lock(&map->lock);
#endif
    uint32_t hash_full = hash_func(key, key_size);
    uint32_t hash = hash_full >> (32 - map->size_log2);
    uint8_t extra = hash_extra(hash_full);
    struct hashmap_entry_t *found = get_from_bucket(&map->buckets[hash], key, key_size, extra);
    if (found == NULL || found->value == NULL) {
#ifdef HASHMAP_THREAD_SAFE
        pthread_mutex_unlock(&map->lock);
#endif
        return false;
    }

    found->value = NULL;
    if (found->alloc_flag)
        free(found->value);
#ifdef HASHMAP_THREAD_SAFE
    pthread_mutex_unlock(&map->lock);
#endif
    map->len--;
    return true;
}

uint32_t hashmap_len(struct hashmap_t *map)
{
    return map->len;
}
