#include <stdlib.h>
#include <stdbool.h>

#include "stack.h"

void stack_init(struct stack_t *stack, size_t capacity)
{
    stack->size = 0;
    stack->capacity = capacity;
    stack->items = (void **)(malloc(capacity * sizeof(void *)));
}

void free_stack(struct stack_t *stack)
{
    free(stack->items);
    free(stack);
}

inline bool stack_empty(struct stack_t *stack)
{
    return !stack->size;
}

inline bool stack_full(struct stack_t *stack)
{
    return stack->size == stack->capacity;
}

inline bool stack_push(struct stack_t *stack, void *item)
{
    if (stack_full(stack))
        return false;

    *(stack->items + stack->size++) = item;
    return true;
}

inline void *stack_pop(struct stack_t *stack)
{
    if (stack_empty(stack))
        return NULL;
        
    return *(stack->items + --stack->size);
}

inline void *stack_get(struct stack_t *stack)
{
    if (stack_empty(stack))
        return NULL;
        
    return *(stack->items + stack->size - 1);
}