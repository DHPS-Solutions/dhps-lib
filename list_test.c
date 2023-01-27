#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "list.h"

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

    int i;
    void *item;
    printf("After adding\n");
    LIST_FOR_EACH(list, i, item) {
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

    printf("Index of item with value 5: %ld\n", list_index_of_item(list, (void *)&e));
    
    printf("Value of item with index 2: %d\n", *(int*)list_item_at_index(list, 2));

    list_delete_all(list);

    printf("After delete all\n");
    LIST_FOR_EACH(list, i, item) {
        printf("%d\n", *(int *)item);        
    }
    
    free_list(list);


    return 0;
}