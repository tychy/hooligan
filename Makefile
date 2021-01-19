CFLAGS=-std=c11 -static

main: main.c
	cc $(CFLAGS) -o exe.out main.c
test: main exe.out
	./test.sh
clean:
	rm -f exe.out *.o *~ tmp*
.PHONY: test clean
