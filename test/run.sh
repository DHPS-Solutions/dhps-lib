#!/bin/sh

success=0
n_tests="$(ls *.c | wc -l)"

for file in *.c
do
    gcc -g -fsanitize=address -fsanitize=undefined -Wall -Wpedantic -Wextra -Werror "$file" ../hashmap.c
    valgrind -s ./a.out >/dev/null
    if ./a.out
    then
        printf "\033[0;32m$file PASSED\033[0m\n\n"
        success=$((success + 1))
    else
        printf "\033[0;31m$file FAILED\033[0m\n\n"
    fi
done

rm a.out

echo "$success" / "$n_tests" tests passed
