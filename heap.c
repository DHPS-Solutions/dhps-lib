#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "heap.h"

static inline void *heap_left_child(struct heap_t *heap, int idx)
{
    return *(heap->items + heap_left_child_idx(idx));
}

static inline void *heap_right_child(struct heap_t *heap, int idx)
{
    return *(heap->items + heap_right_child_idx(idx));
}

static inline void *heap_parent(struct heap_t *heap, int idx)
{
    return *(heap->items + heap_parent_idx(idx));
}

static void ensure_capacity(struct heap_t *heap)
{
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->items = (void **)(realloc(heap->items, heap->capacity * sizeof(void *)));
    }
}

static void heapify_up(struct heap_t *heap)
{
    int idx = heap->size - 1;
    int parent_idx = heap_parent_idx(idx);
    while (parent_idx >= 0 && heap->cmp(*(heap->items + parent_idx), *(heap->items + idx))) {
        swap(*(heap->items + parent_idx), *(heap->items + idx));
        idx = heap_parent_idx(idx);
        parent_idx = heap_parent_idx(idx);
    }
}

static void heapify_down(struct heap_t *heap)
{
    int idx = 0;
    int min_idx;
    while (heap_has_left(idx, heap->size)) {
        min_idx = heap_left_child_idx(idx);
        if (heap_has_right(idx, heap->size) && heap->cmp(*(heap->items + min_idx), 
                                                      heap_right_child(heap, idx)))
            min_idx = heap_right_child_idx(idx);

        if (heap->cmp(*(heap->items + min_idx), *(heap->items + idx))) {
            break;
        } else {
            swap(*(heap->items + idx), *(heap->items + min_idx));
            idx = min_idx;
        }
    }
}

void *heap_get(struct heap_t *heap, int idx)
{
    if (idx < 0 || idx >= heap->size || heap->size == 0)
        return NULL;

    return *(heap->items + idx);
}

void *heap_pop(struct heap_t *heap)
{
    void *item = heap_get(heap, 0);
    if (item == NULL)
        return NULL;

    *(heap->items) = *(heap->items + --heap->size);
    heapify_down(heap);
    return item;
}

void heap_push(struct heap_t *heap, void *item)
{
    ensure_capacity(heap);
    *(heap->items + heap->size++) = item;
    heapify_up(heap);
}

void heap_free(struct heap_t *heap)
{
    free(heap->items);
    free(heap);
}

struct heap_t *heap_malloc(compare_func *cmp)
{
    struct heap_t *heap = (struct heap_t *)(malloc(sizeof(struct heap_t)));
    heap->size = 0;
    heap->capacity = HEAP_STARTING_CAPACITY;
    heap->items = (void **)(malloc(HEAP_STARTING_CAPACITY * sizeof(void *)));
    heap->cmp = cmp;
    return heap;
}
