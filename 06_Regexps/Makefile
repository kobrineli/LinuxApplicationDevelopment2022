esub: esub.c
	clang -o esub $^

.PHONY: clean test

test: esub
	./esub abcd 1234 efgk | cmp test1
	./esub 'ab(cd)' '12\1' 000abcd000 | cmp test2
	./esub '(ab[0-9]*(cd))' '12\1\2' 000ab333cd000 | cmp test3
	./esub 'ab(cd)' '12\\1' abcd | cmp test4

clean:
	rm esub
