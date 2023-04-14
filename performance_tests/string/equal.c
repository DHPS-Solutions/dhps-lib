#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../common.h"
#include "../../string.h"

#define TEST_COUNT 1000000
#define MAX_STR_LENGTH 15

// Stolen random string generator from: William Morris -> https://codereview.stackexchange.com/users/10269/william-morris
static char *rand_string()
{
    int size = rand() % MAX_STR_LENGTH;
    char *str = malloc(size);
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

int main()
{
    printf("------------------------------------------------------\n");
    printf("String equals performance tests.\n");
    printf("Iterations for the tests: %d\n", TEST_COUNT);
    srand(clock());

    char *str1 = rand_string();
    char *str2 = rand_string();
    char *str3 = rand_string();
    char *str4 = rand_string();
    char *str5 = rand_string();
    char *str6 = rand_string();

    string s = string_new(str1);
    string s2 = string_new(str2);
    string s3 = string_new(str3);
    string s4 = string_new(str4);
    string s5 = string_new(str5);
    string s6 = string_new(str6);
    
    int sum = 0;

    clock_t t1 = clock();
    for (int i = 0; i < TEST_COUNT; i++) {
        sum += string_equal(s, s);
        sum += string_equal(s2, s2);
        sum += string_equal(s3, s3);
        sum += string_equal(s4, s4);
        sum += string_equal(s5, s5);
        sum += string_equal(s6, s6);
        sum += string_equal(s, s2);
        sum += string_equal(s, s3);
        sum += string_equal(s, s4);
        sum += string_equal(s, s5);
        sum += string_equal(s, s6);
        sum += string_equal(s2, s3);
        sum += string_equal(s2, s4);
        sum += string_equal(s2, s5);
        sum += string_equal(s2, s6);
        sum += string_equal(s3, s4);
        sum += string_equal(s3, s5);
        sum += string_equal(s3, s6);
        sum += string_equal(s4, s5);
        sum += string_equal(s4, s6);
        sum += string_equal(s5, s6);
    }
    t1 = clock() - t1;

    clock_t t2 = clock();
    for (int i = 0; i < TEST_COUNT; i++) {
        sum += STR_EQ(s, s);
        sum += STR_EQ(s2, s2);
        sum += STR_EQ(s3, s3);
        sum += STR_EQ(s4, s4);
        sum += STR_EQ(s5, s5);
        sum += STR_EQ(s6, s6);
        sum += STR_EQ(s, s2);
        sum += STR_EQ(s, s3);
        sum += STR_EQ(s, s4);
        sum += STR_EQ(s, s5);
        sum += STR_EQ(s, s6);
        sum += STR_EQ(s2, s3);
        sum += STR_EQ(s2, s4);
        sum += STR_EQ(s2, s5);
        sum += STR_EQ(s2, s6);
        sum += STR_EQ(s3, s4);
        sum += STR_EQ(s3, s5);
        sum += STR_EQ(s3, s6);
        sum += STR_EQ(s4, s5);
        sum += STR_EQ(s4, s6);
        sum += STR_EQ(s5, s6);
    }
    t2 = clock() - t2;

    clock_t t3 = clock();
    for (int i = 0; i < TEST_COUNT; i++) {
        sum += strcmp(s->data, s->data) == 0;
        sum += strcmp(s2->data, s2->data) == 0;
        sum += strcmp(s3->data, s3->data) == 0;
        sum += strcmp(s4->data, s4->data) == 0;
        sum += strcmp(s5->data, s5->data) == 0;
        sum += strcmp(s6->data, s6->data) == 0;
        sum += strcmp(s->data, s2->data) == 0;
        sum += strcmp(s->data, s3->data) == 0;
        sum += strcmp(s->data, s4->data) == 0;
        sum += strcmp(s->data, s5->data) == 0;
        sum += strcmp(s->data, s6->data) == 0;
        sum += strcmp(s2->data, s3->data) == 0;
        sum += strcmp(s2->data, s4->data) == 0;
        sum += strcmp(s2->data, s5->data) == 0;
        sum += strcmp(s2->data, s6->data) == 0;
        sum += strcmp(s3->data, s4->data) == 0;
        sum += strcmp(s3->data, s5->data) == 0;
        sum += strcmp(s3->data, s6->data) == 0;
        sum += strcmp(s4->data, s5->data) == 0;
        sum += strcmp(s4->data, s6->data) == 0;
        sum += strcmp(s5->data, s6->data) == 0;
    }
    t3 = clock() - t3;

    printf("Sum: %d\n", sum);
    printf("Function-based: T1: %fms\n", (double)t1/1000);
    printf("Macro-based: T1: %fms\n", (double)t2/1000);
    printf("Libc T2: %fms\n", (double)t3/1000);

    string_free(s);
    string_free(s2);
    string_free(s3);
    string_free(s4);
    string_free(s5);
    string_free(s6);
    free(str1);
    free(str2);
    free(str3);
    free(str4);
    free(str5);
    free(str6);

    printf("Relative speeds (strcmp baseline):\n");
    printf("Function-based: %f\n", (double)t3 / (double)t1);
    printf("Macro-based: %f\n", (double)t3 / (double)t2);
    printf("------------------------------------------------------\n");

    return 0;
}