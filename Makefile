CFLAGS=-std=c11 -static
SOURCE=$(wildcard *.c)
OBJS=$(SOURCE:.c=.o)
main: $(OBJS)
	cc $(CFLAGS) -o exe.out $(OBJS)

$(OBJS): hooligan.h

test: main exe.out
	./test.sh
clean:
	rm -f exe.out *.o *~ tmp*
.PHONY: test clean
