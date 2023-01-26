#ifndef FILEIO_H
#define FILEIO_H
#include <stdlib.h>
#include <stdio.h>

/* Structs */

/* Struct that contains the data of a file as bytes and the length of the data. */
struct file_data_t {
    char *data; /* The file data. */
    size_t data_len; /* The data length. */
};

/* Methods */

/* 
    Method to get data from file as bytes. 
    @param FILE pointer to the file to read.
    @return The file data and data length or NULL if there is an error.
*/
struct file_data_t *read_file(FILE *input);

#endif