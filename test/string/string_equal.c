#include "../../string.h"
#include <string.h>
#include <stdbool.h>
#include <assert.h>

int main()
{
    // Create 3 strings
    string s = string_new("Hello World");
    string s2 = string_new("Hello World");
    string s3 = string_new("Goodbye World");

    bool cmp = false;

    // Assert s is equal to s2
    cmp = string_equal(s, s2);
    assert(cmp == true && strcmp(s->data, s2->data) == 0);

    // Assert s is unequal to s3
    cmp = string_equal(s, s3);
    assert(cmp == false && strcmp(s->data, s3->data) != 0);

    string_free(s);
    string_free(s2);
    string_free(s3);
    
    return 0;
}