#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../list.h"

int main()
{
    struct list_t *list = list_malloc(sizeof(int));

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;

    list_add(list, (void *)&a);
    list_add(list, (void *)&b);
    list_add(list, (void *)&c);
    list_add(list, (void *)&d);

    size_t i;
    void *item;
    printf("After adding\n");
    LIST_FOR_EACH(list, i, item) {
        printf("%d\n", *(int *)item);        
    }

    printf("Reversed\n");
    LIST_REVERSED_FOR_EACH(list, i, item) {
        printf("%d\n", *(int *)item);        
    }

    list_delete_at_index(list, 2);
    printf("After delete index 2\n");
    LIST_FOR_EACH(list, i, item) {
        printf("%d\n", *(int *)item);        
    }

    list_add(list, (void *)&e);
    printf("After add 5\n");
    LIST_FOR_EACH(list, i, item) {
        printf("%d\n", *(int *)item);        
    }

    int *item_T;
    printf("Using type casting\n");
    LIST_FOR_EACH_T(list, i, item_T, int)
        printf("%d\n", *item_T);

    int test;
    printf("Using type casting 2\n");
    #define T int


    printf("Index of item with value 5: %ld\n", list_index_of_item(list, (void *)&e));
    
    printf("Value of item with index 2: %d\n", *(int*)list_item_at_index(list, 2));

    list_delete_all(list);

    printf("After delete all\n");
    LIST_FOR_EACH(list, i, item) {
        printf("%d\n", *(int *)item);        
    }
    
    printf("Test capacity\n");

    for (int i = 0; i < 100; i++) {
        list_add(list, (void *)&i);
    }

    free_list(list);

    return 0;
}
