#!/bin/sh

doTest () {
    echo "Testing $1"
    eval $2 || { printf "\033[31m$1 fail\033[0m\n"; exit 1; }
    printf "\033[32m%s\033[0m\n" "$1 success"
}

doTest "mode" '[ "$(echo 1 1 2 3 | ./stat mode)" = "1 (2)" ]'
doTest "mean" '[ "$(echo 1 2 3 | ./stat mean)" = "2" ]'
doTest "sum" '[ "$(echo 1 -2 0 | ./stat sum)" = "-1" ]'
doTest "max" '[ "$(echo 1 3 2332 32 -32 | ./stat max)" = "2332" ]'

echo "All tests pass"
