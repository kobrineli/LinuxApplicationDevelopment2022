move: move.c
	clang -o $@ $^

.PHONY: clean test test-ret test-protect

test: test-protect test-ret

test-protect: move
	@clang -shared -o protect.so protect.c
	@printf "1234" > PROTECT_in
	LD_PRELOAD=`pwd`/protect.so ./move PROTECT_in out
	test -f PROTECT_in
	@rm PROTECT_in out protect.so

test-ret: move
	./test1.sh
	./test2.sh
	./test3.sh
	./test4.sh
	./test5.sh
	./test6.sh

clean:
	rm move
