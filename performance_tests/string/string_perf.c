#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../../common.h"
#include "../../string.h"

#define TEST_COUNT 1000000

double test_cmp()
{
    char *str1 = "Hello, world!";
    char *str2 = "Hello, world!";
    char *str3 = "Goodbye, world!";
    char *str4 = "hello, world!";
    char *str5 = "Good morning USA!";
    char *str6 = "This is a test string.";

    string s = string_new(str1);
    string s2 = string_new(str2);
    string s3 = string_new(str3);
    string s4 = string_new(str4);
    string s5 = string_new(str5);
    string s6 = string_new(str6);
    
    clock_t t1 = clock();
    for (int i = 0; i < TEST_COUNT; i++) {
        string_compare(s, s2);
        string_compare(s, s3);
        string_compare(s, s4);
        string_compare(s, s5);
        string_compare(s, s6);
        string_compare(s2, s3);
        string_compare(s2, s4);
        string_compare(s2, s5);
        string_compare(s2, s6);
        string_compare(s3, s4);
        string_compare(s3, s5);
        string_compare(s3, s6);
        string_compare(s4, s5);
        string_compare(s4, s6);
        string_compare(s5, s6);
    }
    t1 = clock() - t1;

    clock_t t2 = clock();
    for (int i = 0; i < TEST_COUNT; i++) {
        strcmp(s->data, s2->data);
        strcmp(s->data, s3->data);
        strcmp(s->data, s4->data);
        strcmp(s->data, s5->data);
        strcmp(s->data, s6->data);
        strcmp(s2->data, s3->data);
        strcmp(s2->data, s4->data);
        strcmp(s2->data, s5->data);
        strcmp(s2->data, s6->data);
        strcmp(s3->data, s4->data);
        strcmp(s3->data, s5->data);
        strcmp(s3->data, s6->data);
        strcmp(s4->data, s5->data);
        strcmp(s4->data, s6->data);
        strcmp(s5->data, s6->data);
    }
    t2 = clock() - t2;

    printf("T1: %f\n", (double)t1);
    printf("T2: %f\n", (double)t2);

    string_free(s);
    string_free(s2);
    string_free(s3);
    string_free(s4);
    string_free(s5);
    string_free(s6);

    return (double)t2/(double)t1;
}

int main(int argc, char **argv)
{
    string s = string_new("Hello, world!");
    string s2 = string_new("Hello, world!");
    printf("%s\n", s->data);

    int compare = string_compare(s, s2);
    if (compare == CMP_ERR)
        printf("Error comparing strings.\n");
    else if (compare == 0)
        printf("Strings are equal.\n");
    else if (compare < -1)
        printf("String 1 is more alphabetical than destination string.\n");
    else if (compare > 1)
        printf("String 2 is more alphabetical than destination string.\n");

    string s3 = string_concat(s, s2);
    printf("%s\n", s2->data);
    printf("%s\n", s3->data);

    compare = string_compare(s2, s3);
    if (compare == CMP_ERR)
        printf("Error comparing strings.\n");
    else if (compare == 0)
        printf("Strings are equal.\n");
    else if (compare < -1)
        printf("String 1 is more alphabetical than string 2 string.\n");
    else if (compare > 1)
        printf("String 2 is more alphabetical than string 1 string.\n");

    string_free(s);
    string_free(s2);

    double t1 = test_equal();
    printf("Equal is %f the speed of strcmp.\n", t1);

    double t2 = test_cmp();
    printf("Compare is %f the speed of strcmp.\n", t2);

    return 0;
}