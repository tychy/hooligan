CFLAGS=-std=c11 -static
SOURCE=$(wildcard *.c)
OBJS=$(SOURCE:.c=.o)
main: $(OBJS)
	cc $(CFLAGS) -o bin/hcc $(OBJS)

$(OBJS): hooligan.h

test:
	./bin/hdb test all
clean:
	rm -f bin/hcc *.o *~ tmp*
.PHONY: test clean
