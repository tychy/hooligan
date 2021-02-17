CFLAGS=-std=c11 -static
SOURCE=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
main: $(SOURCE)
	cc $(CFLAGS) -o exe.out $(SOURCE)
test: main exe.out
	./test.sh
clean:
	rm -f exe.out *.o *~ tmp*
.PHONY: test clean
