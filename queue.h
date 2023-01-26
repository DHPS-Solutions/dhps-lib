#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>

/* Structs */

/* Struct for a generic queue. */
struct queue_t {
    void    **items; /* void **: The items in the queue. */
    int     start; /* int: The start position of the queue. */
    int     end; /* int: The end position of the queue. */
    size_t  size; /* size_t: The amount of items in the queue. */
    size_t  max; /* size_t: The max amount of items that can be in the queue. */
};

/* Methods */

/* 
    Method to allocate memory for a new queue.
    @param size size_t: the max size of the queue.
    @returns a pointer to a new queue. 
*/
struct queue_t *malloc_queue(size_t size);

/* 
    Destructor method for a queue.
    @param queue struct queue_t *: the queue to be destroyed.
*/
void free_queue(struct queue_t *queue);

/* 
    Predicate method to check if queue is empty.
    @param queue struct queue_t *: the queue to check.
    @returns bool of the queue's empty state.
*/
bool queue_empty(struct queue_t *queue);

/* 
    Predicate method to check if queue is empty.
    @param queue struct queue_t *: the queue to check.
    @returns bool of the queue's full state.
*/
bool queue_full(struct queue_t *queue);

/* 
    Predicate method to check if queue is empty.
    @param queue struct queue_t *: the queue to push to.
    @param item void*: the item to push.
    @returns bool if the item was successfully added.
*/
bool queue_push(struct queue_t *queue, void *item);

/* 
    Method to get the first item in the queue and remove it. 
    @param queue struct queue_t *: the queue to get the item from.
    @returns a void pointer to the item or NULL queue is empty.
*/
void *queue_pop(struct queue_t *queue);

#endif