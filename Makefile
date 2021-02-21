CFLAGS=-std=c11 -static
SOURCE=$(wildcard *.c)
OBJS=$(SOURCE:.c=.o)
main: $(OBJS)
	cc $(CFLAGS) -o bin/exe.out $(OBJS)

$(OBJS): hooligan.h

test: main bin/exe.out
	./test.sh
clean:
	rm -f bin/exe.out *.o *~ tmp*
.PHONY: test clean
