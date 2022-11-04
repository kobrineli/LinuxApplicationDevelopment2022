#!/bin/bash

printf "1234" > a
strace -e trace=openat -e fault=openat:error=EPERM:when=4 ./move a b 2> /dev/null
test $? -eq 3 && test -f a || echo "Test 2 failed."
rm -f a b
