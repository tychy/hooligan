CFLAGS=-std=c11 -static
SOURCE=$(wildcard *.c)
OBJS=$(SOURCE:.c=.o)
main: $(OBJS)
	cc $(CFLAGS) -o bin/hcc $(OBJS)

$(OBJS): hooligan.h

clean:
	rm -f bin/hcc *.o *~ tmp*
.PHONY: test clean
