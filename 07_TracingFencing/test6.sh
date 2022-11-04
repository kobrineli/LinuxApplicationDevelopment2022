#!/bin/bash

printf "1234" > a
strace -e trace=unlink -e fault=unlink:error=EPERM:when=1 ./move a b 2> /dev/null
test $? -eq 11 && test -f a || echo "Test 6 failed."
rm -f a b
