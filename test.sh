#!/bin/bash

assert() {
  expected="$1"
  input="$2"

  ./bin/exe.out "$input" > tmp.s
  cc -o tmp tmp.s helper/helper.c
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 22 "int main(){ 5+21-4; }"
assert 131 "int main(){ 50+121-40; }"
assert 131 "int main(){ 50    +   121-40; }"
assert 41 "int main(){  12 + 34 - 5 ; }"
assert 10 "int main(){ 2 * 5; }"
assert 42 "int main(){ 7 * 2 * 3; }"
assert 13 "int main(){ 1 + 3 * 4; }"
assert 6 "int main(){ 5 - 2 * 3 + 7; }"
assert 37 "int main(){ 4 * 5 * 2 + 6 * 7 - 5 * 9; }"
assert 5 "int main(){ 5*1*1*1*1*1*1*1*1*1; }"
assert 2 "int main(){ 8 / 4; }"
assert 5 "int main(){ 60 / 11; }"
assert 6 "int main(){ 18 / 6 + 1 * 3; }"
assert 47 "int main(){ 5+6*7; }"
assert 15 "int main(){ 5*(9-6); }"
assert 4 "int main(){ (3+5)/2; }"
assert 32 "int main(){ ( 4 * 9) - (6 * 5) / 7; }"
assert 0 "int main(){ 1 + -3 + 2; }"
assert 1 "int main(){ -(-1); }"
assert 0 "int main(){ 3 + -3; }"
assert 20 "int main(){ -5 * -4; }"
assert 5 "int main(){ -10 / -2; }"
assert 1 "int main(){ 1==1; }"
assert 0 "int main(){ 1==2; }"
assert 1 "int main(){ 4*6 == 3*8; }"
assert 1 "int main(){ 500 * 100 == 250 * 200; }"
assert 7 "int main(){ (1 == 1) + 6; }"
assert 1 "int main(){ 0!=1; }"
assert 0 "int main(){ 5!=2+3; }"
assert 1 "int main(){ 5>=2+3; }"
assert 0 "int main(){ 5>2+3; }"
assert 1 "int main(){ 5<=2+3; }"
assert 0 "int main(){ 5<2+3; }"
assert 1 "int main(){ 2>1; }"
assert 0 "int main(){ 2<1; }"
assert 1 "int main(){ 2>=1; }"
assert 0 "int main(){ 2<=1; }"
assert 1 "int main(){ (4 > 1) >= (1 < 0); }"
assert 1 "int main(){ (4 > 1) > (1 < 0); }"
assert 0 "int main(){ (4 > 1) <= (1 < 0); }"
assert 0 "int main(){ (4 > 1) < (1 < 0); }"
assert 0 "int main(){ int a;a = 0; }"
assert 3 "int main(){ int a;a = 3; }"
assert 3 "int main(){ int a; int b;a = b = 3; }"
assert 5 "int main(){ 1+2;2+3; }"
assert 6 "int main(){ 1;1;1;1;1;1;1;1;1;3+3; }"
assert 2 "int main(){ int a;a=2;a; }"
assert 5 "int main(){ int b; int c;b=2;c=3;b+c; }"
assert 5 "int main(){int foo;int bar; foo=2;bar=3;foo+bar; }"
assert 27 "int main(){int foo; int bar;int hogefuga; foo=2;bar=3;hogefuga=9;hogefuga*bar; }"
assert 1 "int main(){ return 1; }"
assert 2 "int main(){ return 2; return 3; return 4; }"
assert 5 "int main(){int returnx;int a; returnx = 15; a = 5; a; }"
assert 90 "int main(){int a; int b; a = 10; b = 20; return (a+b)*3; }"
assert 2 "int main(){ if(1)2; }"
assert 3 "int main(){ if(0)2; else 3; }"
assert 2 "int main(){int a; a=3;if(a==3)2; else 3; }"
assert 12 "int main(){int a;int i; a=2;for(i=0; i<10; i=i+1) a=a+1; return a; }"
assert 20 "int main(){int counter; for(counter=0;counter<100;counter=counter+1) if(counter*counter==400) return counter; return 1; }"
assert 1 "int main(){int d; for(d=0;d<100;d=d+1) if(d*d==401) return d; return 1; }"
assert 100 "int main(){int a;int i; int j; a=0;for(i=0;i<10;i=i+1) for(j=0;j<10;j=j+1) a = a+1; return a; }"
assert 3 "int main(){int a; a=1; if(a==0)if(a!=0)return 0; else return 4; else return 3; }"
assert 10 "int main(){int i; for(i=0;i<10;) i=i+1;i; }"
assert 10 "int main(){int i; i=0;for(;i<10;) i=i+1;i; }"
assert 10 "int main(){int i; i=0;for(;;) if(i==10)return i;else i=i+1;i; }"
assert 10 "int main(){int i; i=0;while(i<10) i = i+1;i; }"
assert 5 "int main(){int i; i=0;while(i<10) if(i==5) return i;else i = i+1;i; }"
assert 10 "int main(){int i; i=0;while(i<10) while(i<10)i=i+1;i; }"
assert 1 "int main(){ {1;} }"
assert 12 "int main(){int i; i=0;{i=i+2;i=i*6;} }"
assert 1 "int main(){ hello_world(); }"
assert 10 "int main(){ foo(); } int foo(){return 10;}"
assert 6 "int main(){ two(3); }int two(int x) return 2 * x;"
assert 48 "int main(){ two(24); }int two(int hogehoge) { hogehoge * 2; }"
assert 148 "int main(){ two(5 * 20 - 2 * (6+7)); }int two(int y){return 2*y;}"
assert 43 "int main(){ BIG(33); }int BIG(int x)x+10;"
assert 25 "int main(){ add(12,13); }int add(int x,int y){ return x+y; }"
assert 68 "int main(){ add_tri(4, 50, 14); }int add_tri(int a,int b,int c) a+b+c;"
assert 10 "int main(){ add_qua(1,3,2,4); }int add_qua(int a,int b,int c,int d) a+b+c+d;"
assert 17 "int main(){ add_pen(-100,200,57,-117,-23); }
           int add_pen(int i,int j,int k,int l,int m) i+j+k+l+m;"
assert 23 "int main(){ add_hex(23,19,-9,-15,5,0); } 
           int add_hex(int one,int two,int three,int four,int five,int six) one+two+three+four+five+six;"
assert 30 "int main(){ hoge(); } int hoge(){ return 30; }"
assert 1 "int main(){ fibo(1); }
          int fibo(int index) {if(index==0)return 0; if(index==1)return 1; return fibo(index-1)+fibo(index-2);}"
assert 5 "int main(){ fibo(5); }
          int fibo(int index) {if(index==0)return 0; if(index==1)return 1; return fibo(index-1)+fibo(index-2);}"
assert 21 "int main(){ fibo(8); }
          int fibo(int index) {if(index==0)return 0; if(index==1)return 1; return fibo(index-1)+fibo(index-2);}"
assert 55 "int main(){ fibo(10); }
          int fibo(int index) {if(index==0)return 0; if(index==1)return 1; return fibo(index-1)+fibo(index-2);}"
#assert 11 "int main(){a=11;return &a;}"
#assert 12 "int main(){a=12;return *8;}"
assert 12 "int main(){int a;int b;a=12;b=&a;return *b;}"
assert 15 "int main(){int a; int b; int c;a=12;b=15;c= &a - 8;return *c;}"
rm tmp
rm tmp.s

echo OK
