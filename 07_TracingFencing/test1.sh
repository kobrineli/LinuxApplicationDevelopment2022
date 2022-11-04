#!/bin/bash

printf "1234" > a
strace -e trace=openat -e fault=openat:error=EPERM:when=3 ./move a b 2> /dev/null
test $? -eq 2 && test -f a || echo "Test 1 failed."
rm -f a b
