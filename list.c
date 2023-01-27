#include <stdlib.h>
#include <stdbool.h>

#include "list.h"

static void __ensure_capacity(struct list_t *list)
{
    if (list->size == list->capacity) {
        list->capacity <<= 1;
        list->items = (void **)(realloc(list->items, list->capacity * sizeof(void *)));
    }
}

static void __reduce_capacity(struct list_t *list)
{
    if (list->size < (list->capacity >> 1)) {
        list->capacity >>= 1;
        list->items = (void **)(realloc(list->items, list->capacity * sizeof(void *)));
    }
}

bool list_empty(struct list_t *list)
{
    return (bool)list->items;
}

void list_add(struct list_t *list, void *item)
{
    __ensure_capacity(list);
    *(list->items + list->size++) = item;
}

inline void *list_item_at_index(struct list_t *list, size_t idx)
{
    if (idx > list->size)
        return NULL;
    
    return *(list->items + idx);
}

long list_index_of_item(struct list_t *list, const void *item)
{
    bool pred = true;
    long i = 0;
    while (i < list->size) {
        BYTES_EQUAL((char *)*(list->items + i), (char *)item, list->item_size, pred);
        if (pred) 
            break;
        i++;
    }
    
    if (!pred)
        i = ITEM_NOT_FOUND;

    return i;
}

long list_delete_at_index(struct list_t *list, size_t idx)
{
    if (idx > list->size)
        return ITEM_NOT_FOUND;

    for (long i = idx; i < list->size - 1; i++)
        *(list->items + i) = *(list->items + i + 1);

    --list->size;

    __reduce_capacity(list);

    return idx;
}

long list_delete_item(struct list_t *list, const void *item)
{
    long ret;
    if ((ret = list_index_of_item(list, item)) == ITEM_NOT_FOUND)
        return ITEM_NOT_FOUND;
    
    for (long i = ret; i < list->size - 1; i++)
        *(list->items + i) = *(list->items + i + 1);
    
    --list->item_size;

    __reduce_capacity(list);

    return ret;
}

struct list_t *list_malloc(size_t item_size)
{
    struct list_t *list = (struct list_t *)(malloc)(sizeof(struct list_t));

    list->size = 0;
    list->capacity = LIST_STARTING_CAPACITY;
    list->items = (void **)(malloc(LIST_STARTING_CAPACITY * sizeof(void *)));
    list->item_size = item_size;

    return list;
}

void list_delete_all(struct list_t *list)
{
    list->size = 0;
    list->capacity = LIST_STARTING_CAPACITY;
    list->items = (void **)(realloc(list->items, list->capacity * sizeof(void *)));
}

void free_list(struct list_t *list)
{
    free(list->items);
    free(list);
}