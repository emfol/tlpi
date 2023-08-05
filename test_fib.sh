#!/bin/sh

bin='./fib.exe'
printf 'Testing: %s\n' "${bin}"
( for i in $(seq 0 32); do "${bin}" "${i}"; done ) >fib-test_result.txt 2>&1
cmp fib-test_result.txt fib-expected_result.txt && echo OK || echo FAIL
