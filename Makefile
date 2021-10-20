CFLAGS:=-std=c11 -no-pie -MMD
SOURCE:=$(wildcard src/*.c)
OBJS:=$(SOURCE:.c=.o)
DEPENDS:=$(SOURCE:.c=.d)


# 第一世代コンパイラの作成
first: $(OBJS)
	@cc $(CFLAGS) -o bin/hcc1 $(OBJS)

$(OBJS): src/hooligan.h

-include $(DEPENDS)

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
	find *.s | xargs cc -no-pie -o $(OUTPUT) && \
	rm *.s

clean:
	@rm -f bin/hcc* *.o src/*.o *~ tmp* *.s a.out core.*
.PHONY: test clean selfhost
