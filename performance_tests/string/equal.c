#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../common.h"
#include "../../string.h"

#define TEST_COUNT 1000000

double test_equal()
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
        string_equal(s, s2);
        string_equal(s, s3);
        string_equal(s, s4);
        string_equal(s, s5);
        string_equal(s, s6);
        string_equal(s2, s3);
        string_equal(s2, s4);
        string_equal(s2, s5);
        string_equal(s2, s6);
        string_equal(s3, s4);
        string_equal(s3, s5);
        string_equal(s3, s6);
        string_equal(s4, s5);
        string_equal(s4, s6);
        string_equal(s5, s6);
    }
    t1 = clock() - t1;

    clock_t t2 = clock();
    for (int i = 0; i < TEST_COUNT; i++) {
        strcmp(s->data, s2->data) == 0;
        strcmp(s->data, s3->data) == 0;
        strcmp(s->data, s4->data) == 0;
        strcmp(s->data, s5->data) == 0;
        strcmp(s->data, s6->data) == 0;
        strcmp(s2->data, s3->data) == 0;
        strcmp(s2->data, s4->data) == 0;
        strcmp(s2->data, s5->data) == 0;
        strcmp(s2->data, s6->data) == 0;
        strcmp(s3->data, s4->data) == 0;
        strcmp(s3->data, s5->data) == 0;
        strcmp(s3->data, s6->data) == 0;
        strcmp(s4->data, s5->data) == 0;
        strcmp(s4->data, s6->data) == 0;
        strcmp(s5->data, s6->data) == 0;
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
    printf("------------------------------------------------------\n");
    printf("String equals performance tests.\n");
    printf("Equal is %f the speed of strcmp.\n", test_equal());
    printf("------------------------------------------------------\n");

    return 0;
}