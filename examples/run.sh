#!/bin/sh

for f in *_test.c
do
    gcc -g -fsanitize=address -fsanitize=undefined -Wall -Wpedantic -Wextra -Werror
done
