#!/bin/bash

printf "1234" > a
strace -e trace=close -e fault=close:error=EPERM:when=4 ./move a b 2> /dev/null
test $? -eq 10 && test -f a || echo "Test 5 failed."
rm -f a b
