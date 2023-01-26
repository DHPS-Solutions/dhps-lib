#include <stdlib.h>
#include <stdio.h>

#include "fileio.h"

struct file_data_t *read_file(FILE *input)
{
    struct file_data_t *res = (struct file_data_t *)
                                (malloc(sizeof(struct file_data_t)));

    fseek(input, 0, SEEK_END);

    res->data_len = ftell(input);

    rewind(input);

    res->data = (char *)(malloc(res->data_len * sizeof(char)));

    if (fread(res->data, res->data_len, 1, input) != res->data_len);
        return NULL;
    
    return res;
}