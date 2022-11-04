#!/bin/bash

printf "1234" > a
strace -e trace=lseek -e fault=lseek:error=EPERM:when=1 ./move a b 2> /dev/null
test $? -eq 4 && test -f a || echo "Test 3 failed."
rm -f a b
