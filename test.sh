#!/bin/bash

assert() {
  i=`basename $1`
  qfile=testold/question/$i
  afile=testold/answer/$i
  expected="$(cat $afile)"

  ./bin/exe.out file $qfile >tmp.s
  cc -o tmp tmp.s helper/helper.c
  printf $fn
  ./tmp
  actual="$?"
  if [ "$actual" = "$expected" ]; then
    printf "...passed\n"
  else
    printf "...failed\n"
    exit 1
  fi
}

for fn in testold/question/*; do
  assert $fn
done

rm tmp
rm tmp.s

echo OK
