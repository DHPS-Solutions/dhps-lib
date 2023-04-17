#include "../../string.h"
#include <string.h>
#include <assert.h>

int main()
{
    // Create 2 strings
    string s = string_new("Hello");
    string s2 = string_new(", world!");

    // Receive the pointer to string s
    string s3 = string_concat(s, s2);

    // Assert they are correct
    assert(strcmp(s->data, "Hello, world!") == 0);
    assert(strcmp(s3->data, "Hello, world!") == 0);

    // Assert they are the same pointer
    assert(*(long *)s == *(long *)s3);

    string_free(s);
    string_free(s2);
    
    return 0;
}