CFLAGS:=-std=c11 -no-pie -MMD
LIBFLAGS:=-lhooligan -L./bin
SOURCE:=$(wildcard src/*.c)
OBJS:=$(SOURCE:.c=.o)
DEPENDS:=$(SOURCE:.c=.d)
LIBSOURCE:=$(wildcard lib/*.c)
LIBOBJS:=$(LIBSOURCE:.c=.o)

# 第一世代コンパイラの作成
first: $(OBJS) lib
	@cc $(OBJS) $(CFLAGS) $(LIBFLAGS) -o bin/hcc1  
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
	cc ./*.s $(CFLAGS) $(LIBFLAGS) -o $(OUTPUT) && \
	rm *.s

lib: $(LIBOBJS)
	@cc -shared -std=c11 -o bin/libhooligan.so $(LIBOBJS)
$(LIBOBJS):

clean:
	@rm -f bin/hcc* *.o src/*.o *~ tmp* *.s a.out core.* bin/*.so
.PHONY: test clean selfhost lib
