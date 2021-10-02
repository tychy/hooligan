CFLAGS:=-std=c11 -static
SOURCE:=$(wildcard src/*.c)
OBJS:=$(SOURCE:.c=.o)
COMPILER_GENERATION:=1


main: $(OBJS)
	cc $(CFLAGS) -o bin/hcc1 $(OBJS)

$(OBJS): src/hooligan.h

clean:
	rm -f bin/hcc* *.o *~ tmp*
.PHONY: test clean
