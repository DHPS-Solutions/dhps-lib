#ifndef LIST_H
#define LIST_H

#ifndef DYNAMIC_MACROS
#define DYNAMIC_MACROS
#endif

#include <stdlib.h>
#include <stdbool.h>

/* Defines */

/* Starting capacity for array lists. */
#define LIST_STARTING_CAPACITY 32

/* Error code if an item does not exists in the array list. */
#define ITEM_NOT_FOUND -1

/**
 * Macro to perform a for each loop for the arraylist.
 * @param l The arraylist to iterate over.
 * @param i The index variable.
 * @param item The item variable. 
 */
#define LIST_FOR_EACH(l, i, item) \
	for ((i) = 0, item = *(l)->items; (i) < (l)->size; item = *((l)->items + ++(i)))

/**
 * Macro to perform a for each loop for the arraylist with a type.
 * @param l The arraylist to iterate over.
 * @param i The index variable.
 * @param item The item variable. 
 * @param T The type of the item.
 */
#define LIST_FOR_EACH_T(l, i, item, T) \
	for ((i) = 0, item = (T *)(*(l)->items); (i) < (l)->size; item = (T *)(*((l)->items + ++(i))))

/**
 * Macro to perform a for each loop for the arraylist in reverse.
 * @param l The arraylist to iterate over.
 * @param i The index variable.
 * @param item The item variable. 
 */
#define LIST_REVERSED_FOR_EACH(l, i, item) \
	for ((i) = 0, item = *((l)->items + (l)->size - 1); (i) < (l)->size; item = *((l)->items + (l)->size - 1 - ++(i)))             

/* Structs */

/**
 * Struct to represent an array list.
 * @param size the current size of the array list.
 * @param capacity the current max memory of the array list.
 * @param item_size the size of each item in the list.
 * @param items the items in the array list.
*/
struct list_t {
    size_t size;
    size_t capacity;
    size_t item_size;
    void **items;
};

/* Methods */

/**
 * Method to initialize an array list.
 */
void init_list(struct list_t *list, size_t item_size);

/* 
    Destructor method for a array list.
    @param list struct list_t *: the array list to be destroyed.
*/
/**
 * Method to free an array list.
 * @param list the array list to be freed.
 */
void free_list(struct list_t *list);

/**
 * Method to add an item to an array list.
 * @param list the array list to add the item to.
 * @param item the item to add to the array list. 
 */
void list_add(struct list_t *list, void *item);

/**
 * Method to get the item at an index in an array list.
 * @param list the array list to get the index from.
 * @param idx the index to get the item from.
 */
void *list_item_at_index(struct list_t *list, size_t idx);

/**
 * Method to get the index of an item in an array list.
 * @param list the array list to get the index from.
 * @param item the item to get the index of.
 * @return the index of the item, or ITEM_NOT_FOUND if the item does not exist.
 */
long list_index_of_item(struct list_t *list, const void *item);

/**
 * Method to delete an item from an array list.
 * @param list the array list to delete the item from.
 * @param item the item to delete.
 * @return the index of the item, or ITEM_NOT_FOUND if the item does not exist.
 */
long list_delete_item(struct list_t *list, const void *item);

/**
 * Method to delete an item at an index from an array list.
 * @param list the array list to delete the item from.
 * @param idx the index to delete the item from.
 * @return the index of the item, or ITEM_NOT_FOUND if the item does not exist.
 */
long list_delete_at_index(struct list_t *list, size_t idx);

/**
 * Method to delete all items from an array list.
 * @param list the array list to delete the items from.
 */
void list_delete_all(struct list_t *list);

/**
 * Method to check if an array list is empty.
 * @param list the array list to check.
 * @return true if the array list is empty, false otherwise.
 */
bool list_empty(struct list_t *list);

#endif
