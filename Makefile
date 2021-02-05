CFLAGS=-std=c11 -static
SOURCE=main.c tokenizer.c parser.c

main: $(SOURCE)
	cc $(CFLAGS) -o exe.out $(SOURCE)
test: main exe.out
	./test.sh
clean:
	rm -f exe.out *.o *~ tmp*
.PHONY: test clean
