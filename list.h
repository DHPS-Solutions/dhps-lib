#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Defines */

/* Starting capacity for array lists. */
#define LIST_STARTING_CAPACITY 32

/* Error code if an item does not exists in the array list. */
#define ITEM_NOT_FOUND -1

/* Macro that checks if bytes are equal a to value b. */
#define BYTES_EQUAL(a, b, size, pred)   \
    do {                                \
        size_t __size = (size);         \
        char *__a = (a);                \
        char *__b = (b);                \
        do {                            \
            pred = *__a++ == *__b++;    \
        } while(pred && --__size > 0);  \
    } while(0)                          

/* Structs */

/* Basic generic dynamic array list. */
struct list_t {
    size_t  size;       /* The current size of the array list. */
    size_t  capacity;   /* The current max memory of the array list. */
    size_t  item_size;  /* The size of each item in the list. */
    void    **items;    /* The items in the array list. */
};

/* Methods */

/*  
    Method to allocate memory for a new array list.
    @returns a pointer to a new array list. 
*/
struct list_t *list_malloc(size_t item_size);

/* 
    Destructor method for a array list.
    @param list struct list_t *: the array list to be destroyed.
*/
void free_list(struct list_t *list);

/* 
    Method to add an item into the array list. 
    @param list struct list_t *: the array list to add the item to.
    @param item void *: a void pointer to the item to add.
*/
void list_add(struct list_t *list, const void *item);

/* 
    Method to get an item from an index in an array list. 
    @param list struct list_t *: the array list to get the item from.
    @param idx size_t: a the index to get.
    @returns a void pointer to the item at the index or null if idx out of bounds. 
*/
void *list_item_at_index(struct list_t *list, size_t idx);

/* 
    Method to get an item from an index in an array list. 
    @param list struct list_t *: the array list to get the index from.
    @param item void *: a void pointer to the item to get.
    @returns the index of the item or -1 if not found.
*/
long list_index_of_item(struct list_t *list, const void *item);

/* 
    Method to get delete item from an array list. 
    @param list struct list_t *: the array list to delete the item from.
    @param item void *: a void pointer to the item to delete.
    @returns the index of the item or -1 if not found.
*/
long list_delete_item(struct list_t *list, const void *item);

/* 
    Method to delete an item from an index in an array list. 
    @param list struct list_t *: the array list to delete the item from.
    @param idx size_t: a the index to delete.
    @returns the index of the item or -1 if not found.
*/
long list_delete_at_index(struct list_t *list, size_t idx);

/*
    Method to delete all items (free's them) from an array list.
    This method also resizes the array to start size.
    @param list struct list_t *: the array list to delete all items from.
*/
void list_delete_all(struct list_t *list);

/* 
    Predicate method to check if an array list is empty. 
    @param list struct list_t *: the array list to check.
    @returns the empty state of the list.
*/
bool list_empty(struct list_t *list);
