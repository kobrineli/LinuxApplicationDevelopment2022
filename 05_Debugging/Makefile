range: range.c
	clang -O0 -g -o $@ $^

.PHONY: clean test

test: range
	gdb --args range 1 12 < scenario1 | grep @@@ -A 4 | cmp correct1
	gdb --args range -100 100 3 < scenario2 | grep @@@ -A 4 | cmp correct2

clean:
	rm range
