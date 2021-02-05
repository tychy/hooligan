#!/bin/bash

assert() {
  expected="$1"
  input="$2"

  ./exe.out "$input" > tmp.s
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 0
assert 42 42
assert 22 "5+21-4"
assert 131 "50+121-40"
assert 131 "50    +   121-40"
assert 41 " 12 + 34 - 5 "
assert 10 "2 * 5"
assert 42 "7 * 2 * 3"
assert 13 "1 + 3 * 4"
assert 6 "5 - 2 * 3 + 7"
assert 37 "4 * 5 * 2 + 6 * 7 - 5 * 9"
assert 5 "5*1*1*1*1*1*1*1*1*1"
assert 2 "8 / 4"
assert 5 "60 / 11"
assert 6 "18 / 6 + 1 * 3"
assert 47 "5+6*7"
assert 15 "5*(9-6)"
assert 4 "(3+5)/2"
assert 32 "( 4 * 9) - (6 * 5) / 7"
assert 0 "1 + -3 + 2"
assert 1 "-(-1)"
assert 0 "3 + -3"
assert 20 "-5 * -4"
assert 5 "-10 / -2"
rm tmp
rm tmp.s

echo OK
