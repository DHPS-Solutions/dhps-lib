#include "../../string.h"
#include <string.h>
#include <assert.h>

int main()
{
    // Create 3 strings
    string s = string_new("Hello World");
    string s2 = string_new("Hello World");
    string s3 = string_new("Goodbye World");

    int cmp = 0;

    // Assert they are equal
    cmp = string_compare(s, s2);
    assert(cmp == 0 && cmp == strcmp(s->data, s2->data));

    // Assert s is less alphabetical than s3
    cmp = string_compare(s, s3);
    assert(cmp > 0 && cmp == strcmp(s->data, s3->data));

    // Assert s3 is more alphabetical than s
    cmp = string_compare(s3, s);
    assert(cmp < 0 && cmp == strcmp(s3->data, s->data));

    string_free(s);
    string_free(s2);
    string_free(s3);
    
    return 0;
}