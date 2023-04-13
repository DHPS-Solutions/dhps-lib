#include <stdio.h>
#include "../string.h"


int main()
{
    char *substr = "Hello, World!";
    struct string_t *str = string_new_slice(substr, 0, 4);
    printf("%s\n", str->data);
}
