CFLAGS:=-std=c11 -MMD
LIBFLAGS:=-l hooligan -L./bin
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
	@$(COMPILER) $(SOURCE) && ./bin/assemble.sh && mv a.out $(OUTPUT)

lib: $(LIBOBJS)
	@cc -shared -std=c11 -o bin/libhooligan.so $(LIBOBJS)
$(LIBOBJS): %.o: %.c 
	cc $< $(CFLAGS) -c -fPIC -o $@

clean:
	@rm -f bin/hcc* *.o src/*.o src/*.d lib/*.o lib/*.d *~ tmp* *.s a.out core.* bin/*.so
.PHONY: test clean selfhost lib
