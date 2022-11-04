#!/bin/bash

printf "1234" > a
strace -e trace=close -e fault=close:error=EPERM:when=3 ./move a b 2> /dev/null
test $? -eq 9 && test -f a || echo "Test 4 failed."
rm -f a b
