#!/bin/bash

# TODO This shell scipt should be integrated into hcc
set -eu

lib_path=($(dirname /usr/lib/x86_64-linux-gnu/crti.o))
gcc_lib_path=($(dirname /usr/lib/gcc/x86_64-linux-gnu/*/crtbegin.o))

rm -f ./*.o
find ./*.s | while read -r fn; do
    as -c "$fn" -o "${fn%.s}".o
done
ld -o a.out -m elf_x86_64 "${lib_path[0]}"/crt1.o "${lib_path[0]}"/crti.o "${gcc_lib_path[0]}"/crtbegin.o \
    -L"${lib_path[0]}" -L/usr/lib64 -L/lib64 \
    -L"${gcc_lib_path[0]}" -dynamic-linker /lib64/ld-linux-x86-64.so.2 ./*.o \
    -lc -lgcc --as-needed -lgcc_s --no-as-needed \
    "${gcc_lib_path[0]}"/crtend.o "${lib_path[0]}"/crtn.o \
    -l hooligan -L./bin
