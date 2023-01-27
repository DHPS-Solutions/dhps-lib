#ifndef HEAP__H
#define HEAP__H
#include <stdlib.h>
#include <stdbool.h>

/* Defines */

/*
    The starting capacity of the heap.
*/
#define HEAP_STARTING_CAPACITY 32

/* 
    Macro to find the index of left child to a heap node.
    @param parent_idx int: index of the parent node. 
*/
#define HEAP_LEFT_CHILD_IDX(parent_idx) ((parent_idx << 1) + 1)

/* 
    Macro to find the index of left right to a heap node. 
    @param parent_idx int: index of the parent node. 
*/
#define HEAP_RIGHT_CHILD_IDX(parent_idx) ((parent_idx + 1) << 1)

/* 
    Macro to find the index of the parent to a heap node.
    @param child_idx int: index of the child node. 
*/
#define HEAP_PARENT_IDX(child_idx) ((child_idx - 1) >> 1)

/* 
    Macro to check if the heap node has a left child.
    @param idx int: index of the heap node to check.
    @param size size_t: size of the heap.
*/
#define HEAP_HAS_LEFT(idx, size) (HEAP_LEFT_CHILD_IDX(idx) < size)

/*
    Macro to check if the heap node has a right child.
    @param idx int: index of the heap node to check.
    @param size size_t: size of the heap.
*/
#define HEAP_HAS_RIGHT(idx, size) (HEAP_RIGHT_CHILD_IDX(idx) < size)

/* 
    Macro to swap the place of two items. 
    @param a void *: item a to swap.
    @param b void *: item b to swap.
*/
#define SWAP(a, b)                      \
    do {                                \
        void *c = a;                    \
        a = b;                          \
        b = c;                          \
    } while(0)                          


/* Typedefs and structs for the heap. */

/* 
    Defines a function to compare two items. 
    @param a void *: item a to compare.
    @param b void *: item b to compare.
*/
typedef bool (compare_fn_t)(void *a, void *b);

/* 
    Struct for a generic dynamic heap. 
*/
struct heap_t {
    void            **items; /* void **: The items in the heap. */
    size_t          size; /* size_t: The size of the heap. */
    size_t          capacity; /* size_t: The capacity of the heap. */
    compare_fn_t    *cmp; /* bool(void *, void *): Function to compare two items. */
};

/* Methods for the heap. */

/*  
    Method to allocate memory for a new heap.
    @param cmp bool(void *, void *): the compare function that is to be used.
    @returns a pointer to a new heap. 
*/
struct heap_t *heap_malloc(compare_fn_t *cmp);

/* 
    Destructor method for a heap.
    @param heap struct heap_t *: the heap to be destroyed.
*/
void heap_free(struct heap_t *heap);

/* 
    Method to get an item in the heap without removing it.
    @param heap struct heap_t *: the heap to get the item from.
    @param idx int: the index of the item to get.
    @returns a void pointer to the item or NULL if the index is out of bounds.
*/
void *heap_get(struct heap_t *heap, int idx);

/* 
    Method to get the root item in the heap and remove it. 
    @param heap struct heap_t *: the heap to get the item from.
    @returns a void pointer to the item or NULL heap is empty.
*/
void *heap_pop(struct heap_t *heap);

/* 
    Method to insert an item into the heap. 
    @param heap struct heap_t *: the heap to push the item to.
    @param item void *: a void pointer to the item to push.
*/
void heap_push(struct heap_t *heap, void *item);

#endif
