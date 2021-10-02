int test1() { 5 + 21 - 4; }
int test2() { 50 + 121 - 40; }
int test3() { 50 + 121 - 40; }
int test4() { 12 + 34 - 5; }
int test5() { 2 * 5; }
int test6() { 7 * 2 * 3; }
int test7() { 1 + 3 * 4; }
int test8() { 5 - 2 * 3 + 7; }
int test9() { 4 * 5 * 2 + 6 * 7 - 5 * 9; }
int test10() { 5 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1; }
int test11() { 8 / 4; }
int test12() { 60 / 11; }
int test13() { 18 / 6 + 1 * 3; }
int test14() { 5 + 6 * 7; }
int test15() { 5 * (9 - 6); }
int test16() { (3 + 5) / 2; }
int test17() { (4 * 9) - (6 * 5) / 7; }
int test18() { 1 + -3 + 2; }
int test19() { -(-1); }
int test20() { 3 + -3; }
int test21() { -5 * -4; }
int test22() { -10 / -2; }
int test23() { 1 == 1; }
int test24() { 1 == 2; }
int test25() { 4 * 6 == 3 * 8; }
int test26() { 500 * 100 == 250 * 200; }
int test27() { (1 == 1) + 6; }
int test28() { 0 != 1; }
int test29() { 5 != 2 + 3; }
int test30() { 5 >= 2 + 3; }
int test31() { 5 > 2 + 3; }
int test32() { 5 <= 2 + 3; }
int test33() { 5 < 2 + 3; }
int test34() { 2 > 1; }
int test35() { 2 < 1; }
int test36() { 2 >= 1; }
int test37() { 2 <= 1; }
int test38() { (4 > 1) >= (1 < 0); }
int test39() { (4 > 1) > (1 < 0); }
int test40() { (4 > 1) <= (1 < 0); }
int test41() { (4 > 1) < (1 < 0); }
int test42()
{
    int a;
    a = 0;
}
int test43()
{
    int a;
    a = 3;
}
int test44()
{
    int a;
    int b;
    a = b = 3;
}
int test45()
{
    1 + 2;
    2 + 3;
}
int test46()
{
    1;
    1;
    1;
    1;
    1;
    1;
    1;
    1;
    1;
    3 + 3;
}
int test47()
{
    int a;
    a = 2;
    a;
}
int test48()
{
    int b;
    int c;
    b = 2;
    c = 3;
    b + c;
}
int test49()
{
    int foo;
    int bar;
    foo = 2;
    bar = 3;
    foo + bar;
}
int test50()
{
    int foo;
    int bar;
    int hogefuga;
    foo = 2;
    bar = 3;
    hogefuga = 9;
    hogefuga *bar;
}
int test51() { return 1; }
int test52()
{
    return 2;
    return 3;
    return 4;
}
int test53()
{
    int returnx;
    int a;
    returnx = 15;
    a = 5;
    a;
}
int test54()
{
    int a;
    int b;
    a = 10;
    b = 20;
    return (a + b) * 3;
}
int test55()
{
    if (1)
        2;
}
int test56()
{
    if (0)
        2;
    else
        3;
}
int test57()
{
    int a;
    a = 3;
    if (a == 3)
        2;
    else
        3;
}
int test58()
{
    int a;
    int i;
    a = 2;
    for (i = 0; i < 10; i = i + 1)
        a = a + 1;
    return a;
}
int test59()
{
    int counter;
    for (counter = 0; counter < 100; counter = counter + 1)
        if (counter * counter == 400)
            return counter;
    return 1;
}
int test60()
{
    int d;
    for (d = 0; d < 100; d = d + 1)
        if (d * d == 401)
            return d;
    return 1;
}
int test61()
{
    int a;
    int i;
    int j;
    a = 0;
    for (i = 0; i < 10; i = i + 1)
        for (j = 0; j < 10; j = j + 1)
            a = a + 1;
    return a;
}
int test62()
{
    int a;
    a = 1;
    if (a == 0)
        if (a != 0)
            return 0;
        else
            return 4;
    else
        return 3;
}
int test63()
{
    int i;
    for (i = 0; i < 10;)
        i = i + 1;
    i;
}
int test64()
{
    int i;
    i = 0;
    for (; i < 10;)
        i = i + 1;
    i;
}
int test65()
{
    int i;
    i = 0;
    for (;;)
        if (i == 10)
            return i;
        else
            i = i + 1;
    i;
}
int test66()
{
    int i;
    i = 0;
    while (i < 10)
        i = i + 1;
    i;
}
int test67()
{
    int i;
    i = 0;
    while (i < 10)
        if (i == 5)
            return i;
        else
            i = i + 1;
    i;
}
int test68()
{
    int i;
    i = 0;
    while (i < 10)
        while (i < 10)
            i = i + 1;
    i;
}
int test69()
{
    {
        1;
    }
}
int test70()
{
    int i;
    i = 0;
    {
        i = i + 2;
        i = i * 6;
    }
}
int test72() { foo(); }
int foo() { return 10; }
int two(int x) { return 2 * x; }
int test73() { two(3); }
int test74() { two(24); }
int test75() { two(5 * 20 - 2 * (6 + 7)); }
int BIG(int x) { x + 10; }
int test76() { BIG(33); }
int test77() { add(12, 13); }
int add(int x, int y) { return x + y; }
int test78() { add_tri(4, 50, 14); }
int add_tri(int a, int b, int c) { a + b + c; }
int test79() { add_qua(1, 3, 2, 4); }
int add_qua(int a, int b, int c, int d) { a + b + c + d; }
int test80() { add_pen(-100, 200, 57, -117, -23); }
int add_pen(int i, int j, int k, int l, int m) { i + j + k + l + m; }
int test81() { add_hex(23, 19, -9, -15, 5, 0); }
int add_hex(int one, int two, int three, int four, int five, int six) { one + two + three + four + five + six; }
int test82() { hoge(); }
int hoge() { return 30; }
int test83() { fibo(1); }
int fibo(int index)
{
    if (index == 0)
        return 0;
    if (index == 1)
        return 1;
    return fibo(index - 1) + fibo(index - 2);
}
int test84() { fibo(5); }
int test85() { fibo(8); }
int test86() { fibo(10); }
int test87()
{
    int a;
    int *b;
    a = 12;
    b = &a;
    return *b;
}
int test88()
{
    int a;
    int b;
    int *c;
    a = 12;
    b = 15;
    c = &a - 1;
    return *c;
}
int test89()
{
    int **a;
    int *b;
    int c;
    c = 20;
    b = &c;
    a = &b;
    return **a;
}
int test90()
{
    int x;
    int *y;
    y = &x;
    *y = 77;
    return x;
}
int test91()
{
    int a;
    int b;
    int c;
    int d;
    int *p;
    a = 400;
    b = 15;
    c = 20;
    d = 5;
    p = &a;
    return (*(p - 1) + *(p - 2) + *(p - 3));
}
int test92()
{
    int p;
    int q;
    int *r;
    p = 10;
    q = 20;
    r = &q;
    return *(r + 1);
}
int test93()
{
    int p;
    int q;
    p = 100;
    q = 20;
    return *(&q + 1);
}
int main()
{
    if (test1() != 22)
    {
        return 1;
    }
    if (test2() != 131)
    {
        return 1;
    }
    if (test3() != 131)
    {
        return 1;
    }
    if (test4() != 41)
    {
        return 1;
    }
    if (test5() != 10)
    {
        return 1;
    }
    if (test6() != 42)
    {
        return 1;
    }
    if (test7() != 13)
    {
        return 1;
    }
    if (test8() != 6)
    {
        return 1;
    }
    if (test9() != 37)
    {
        return 1;
    }
    if (test10() != 5)
    {
        return 1;
    }
    if (test11() != 2)
    {
        return 1;
    }
    if (test12() != 5)
    {
        return 1;
    }
    if (test13() != 6)
    {
        return 1;
    }
    if (test14() != 47)
    {
        return 1;
    }
    if (test15() != 15)
    {
        return 1;
    }
    if (test16() != 4)
    {
        return 1;
    }
    if (test17() != 32)
    {
        return 1;
    }
    if (test18() != 0)
    {
        return 1;
    }
    if (test19() != 1)
    {
        return 1;
    }
    if (test20() != 0)
    {
        return 1;
    }
    if (test21() != 20)
    {
        return 1;
    }
    if (test22() != 5)
    {
        return 1;
    }
    if (test23() != 1)
    {
        return 1;
    }
    if (test24() != 0)
    {
        return 1;
    }
    if (test25() != 1)
    {
        return 1;
    }
    if (test26() != 1)
    {
        return 1;
    }
    if (test27() != 7)
    {
        return 1;
    }
    if (test28() != 1)
    {
        return 1;
    }
    if (test29() != 0)
    {
        return 1;
    }
    if (test30() != 1)
    {
        return 1;
    }
    if (test31() != 0)
    {
        return 1;
    }
    if (test32() != 1)
    {
        return 1;
    }
    if (test33() != 0)
    {
        return 1;
    }
    if (test34() != 1)
    {
        return 1;
    }
    if (test35() != 0)
    {
        return 1;
    }
    if (test36() != 1)
    {
        return 1;
    }
    if (test37() != 0)
    {
        return 1;
    }
    if (test38() != 1)
    {
        return 1;
    }
    if (test39() != 1)
    {
        return 1;
    }
    if (test40() != 0)
    {
        return 1;
    }
    if (test41() != 0)
    {
        return 1;
    }
    if (test42() != 0)
    {
        return 1;
    }
    if (test43() != 3)
    {
        return 1;
    }
    if (test44() != 3)
    {
        return 1;
    }
    if (test45() != 5)
    {
        return 1;
    }
    if (test46() != 6)
    {
        return 1;
    }
    if (test47() != 2)
    {
        return 1;
    }
    if (test48() != 5)
    {
        return 1;
    }
    if (test49() != 5)
    {
        return 1;
    }
    if (test50() != 27)
    {
        return 1;
    }
    if (test51() != 1)
    {
        return 1;
    }
    if (test52() != 2)
    {
        return 1;
    }
    if (test53() != 5)
    {
        return 1;
    }
    if (test54() != 90)
    {
        return 1;
    }
    if (test55() != 2)
    {
        return 1;
    }
    if (test56() != 3)
    {
        return 1;
    }
    if (test57() != 2)
    {
        return 1;
    }
    if (test58() != 12)
    {
        return 1;
    }
    if (test59() != 20)
    {
        return 1;
    }
    if (test60() != 1)
    {
        return 1;
    }
    if (test61() != 100)
    {
        return 1;
    }
    if (test62() != 3)
    {
        return 1;
    }
    if (test63() != 10)
    {
        return 1;
    }
    if (test64() != 10)
    {
        return 1;
    }
    if (test65() != 10)
    {
        return 1;
    }
    if (test66() != 10)
    {
        return 1;
    }
    if (test67() != 5)
    {
        return 1;
    }
    if (test68() != 10)
    {
        return 1;
    }
    if (test69() != 1)
    {
        return 1;
    }
    if (test70() != 12)
    {
        return 1;
    }
    if (test72() != 10)
    {
        return 1;
    }
    if (test73() != 6)
    {
        return 1;
    }
    if (test74() != 48)
    {
        return 1;
    }
    if (test75() != 148)
    {
        return 1;
    }
    if (test76() != 43)
    {
        return 1;
    }
    if (test77() != 25)
    {
        return 1;
    }
    if (test78() != 68)
    {
        return 1;
    }
    if (test79() != 10)
    {
        return 1;
    }
    if (test80() != 17)
    {
        return 1;
    }
    if (test81() != 23)
    {
        return 1;
    }
    if (test82() != 30)
    {
        return 1;
    }
    if (test83() != 1)
    {
        return 1;
    }
    if (test84() != 5)
    {
        return 1;
    }
    if (test85() != 21)
    {
        return 1;
    }
    if (test86() != 55)
    {
        return 1;
    }
    if (test87() != 12)
    {
        return 1;
    }
    if (test88() != 15)
    {
        return 1;
    }
    if (test89() != 20)
    {
        return 1;
    }
    if (test90() != 77)
    {
        return 1;
    }
    if (test91() != 40)
    {
        return 1;
    }
    if (test92() != 10)
    {
        return 1;
    }
    if (test93() != 100)
    {
        return 1;
    }

    return 0;
}
