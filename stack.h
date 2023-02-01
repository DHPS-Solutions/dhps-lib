#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdbool.h>

/* Structs */

/* Fixed size generic stack. */
struct stack_t {
    size_t size;
    size_t capacity;
    void **items;
};

/* Methods */

/* 
    Method to initialize stack.
    @param stack struct stack_t *: pointer to the stack to init.
    @param size size_t: the max size of the stack.
*/
void stack_init(struct stack_t *stack, size_t capacity);

/* 
    Destructor method for a stack.
    @param stack struct stack_t *: pointer to the stack to be destroyed.
*/
void free_stack(struct stack_t *stack);

/* 
    Predicate method to check if the stack is empty.
    @param stack struct stack_t *: pointer to the stack to check.
    @returns the empty state of the stack.
*/
bool stack_empty(struct stack_t *stack);

/* 
    Predicate method to check if the stack is full.
    @param stack struct stack_t *: pointer to the stack to check.
    @returns the full state of the stack.
*/
bool stack_full(struct stack_t *stack);


/* 
    Method to insert an item into a stack.
    @param stack struct stack_t *: the stack to push to.
    @param item void*: the item to push.
    @returns bool if the item was successfully added.
*/
bool stack_push(struct stack_t *stack, void *item);

/* 
    Method to get the first item in the stack and remove it. 
    @param stack struct stack_t *: the stack to get the item from.
    @returns a void pointer to the item or NULL stack is empty.
*/
void *stack_pop(struct stack_t *stack);

/* 
    Method to get the first item in the stack. 
    @param stack struct stack_t *: the stack to get the item from.
    @returns a void pointer to the item or NULL stack is empty.
*/
void *stack_get(struct stack_t *stack);

#endif