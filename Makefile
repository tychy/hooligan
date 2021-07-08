CFLAGS=-std=c11 -static
SOURCE=$(wildcard *.c)
OBJS=$(SOURCE:.c=.o)
main: $(OBJS)
	cc $(CFLAGS) -o bin/hcc1 $(OBJS)

$(OBJS): hooligan.h

test:
	./bin/hdb -d false test all
clean:
	rm -f bin/hcc* *.o *~ tmp*
.PHONY: test clean
