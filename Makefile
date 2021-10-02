CFLAGS:=-std=c11 -static
SOURCE:=$(wildcard src/*.c)
OBJS:=$(SOURCE:.c=.o)

# 第一世代コンパイラの作成
first: $(OBJS)
	cc $(CFLAGS) -o bin/hcc1 $(OBJS)
$(OBJS): src/hooligan.h

# 第二世代以降のコンパイラの作成
COMPILER:=
OUTPUT:=bin/hcc
ifneq ($(COMPILER),)
	ifneq ($(shell [ -e $(COMPILER) ] && echo exist),exist)
		$(error 指定されたコンパイラは存在しません)
	endif
else
	COMPILER:=bin/hcc1
endif
selfhost: first
	@$(COMPILER) $(SOURCE) && \
	find *.s | xargs cc -static -o $(OUTPUT) && \
	rm *.s

clean:
	rm -f bin/hcc* *.o *~ tmp*
.PHONY: test clean
