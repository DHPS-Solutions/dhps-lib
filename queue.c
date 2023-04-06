#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"

void init_queue(struct queue_t *queue, int size)
{
    queue->items = (void *)(malloc(size * sizeof(void *)));
    queue->max = size;
    queue->start = queue->end = queue->size = 0;
}

bool queue_empty(struct queue_t *queue)
{
    return !queue->size;
}

bool queue_full(struct queue_t *queue)
{
    return queue->size == queue->max;
}

bool queue_push(struct queue_t *queue, void *item)
{
    if (queue_full(queue))
        return false;
    *(queue->items + queue->end) = item;
    queue->end = (queue->end + 1) % queue->max;
    ++queue->size;
    return true;
}

void *queue_pop(struct queue_t *queue)
{
    void *element;
    if (queue_empty(queue))
        return NULL;

    element = *(queue->items + queue->start);
    queue->start = (queue->start + 1) % queue->max;
    --queue->size;
    return element;
}

void free_queue(struct queue_t *queue)
{
    free(queue->items);
}