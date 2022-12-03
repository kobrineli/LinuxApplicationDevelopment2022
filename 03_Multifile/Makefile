GENERATES = prog prog-a prog-so *.o *.a *.so test*-prog

all: prog prog-a prog-so

%.o: %.c
	clang -fPIC -c -o $@ $<

static: fun.o const.o
	ar -rcs liboutput_static.a $^

shared: fun.o const.o
	clang -shared $^ -oliboutput.so

prog: const.o fun.o prog.o
	clang -o $@ $^

prog-a: prog.o static
	clang -L. -o $@ $< -loutput_static

prog-so: prog.o shared
	clang -L. -o $@ $< -loutput -Wl,-rpath=`pwd`

.PHONY: clean test

test: prog prog-a prog-so
	@echo Testing..
	@./prog 2> test1-prog && cmp tests/test1 test1-prog
	@./prog abc > test2-prog && cmp tests/test2 test2-prog
	@./prog a b c > test3-prog && cmp tests/test3 test3-prog
	@./prog-a 2> test1-prog && cmp tests/test1-a test1-prog
	@./prog-a abc > test2-prog && cmp tests/test2 test2-prog
	@./prog-a a b c > test3-prog && cmp tests/test3 test3-prog
	@./prog-so 2> test1-prog && cmp tests/test1-so test1-prog
	@./prog-so abc > test2-prog && cmp tests/test2 test2-prog
	@./prog-so a b c > test3-prog && cmp tests/test3 test3-prog
	@echo "All tests are passed!"

clean:
	rm -f $(GENERATES)
