#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>

/* Structs */

/**
 * Struct used to represent a queue.
 * @param items The items in the queue.
 * @param start The start of the queue.
 * @param end The end of the queue.
 * @param size The size of the queue.
 * @param max The max size of the queue.
 */
struct queue_t {
    void **items;
    int start;
    int end;
    int size; 
    int max;
};


/* Methods */

/**
 * Function used to initialize a queue.
 * @param queue queue to initialize.
 * @param size size of the queue.
 */
void init_queue(struct queue_t *queue, int size);

/**
 * Function used to check if a queue is empty.
 * @param queue queue to check.
 * @return true if the queue is empty, false otherwise.
 */
bool queue_empty(struct queue_t *queue);

/**
 * Function used to check if a queue is full.
 * @param queue queue to check.
 * @return true if the queue is full, false otherwise.
 */
bool queue_full(struct queue_t *queue);

/**
 * Function used to push an item to a queue.
 * @param queue queue to push to.
 * @param item item to push.
 * @return true if the item was pushed, false otherwise.
 */
bool queue_push(struct queue_t *queue, void *item);

/**
 * Function used to pop an item from a queue.
 * @param queue queue to pop from.
 * @return the item popped from the queue.
 */
void *queue_pop(struct queue_t *queue);

/**
 * Function used to free a queue.
 * @param queue queue to free.
 */
void free_queue(struct queue_t *queue);

#endif