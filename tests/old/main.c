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
    return a;
}
int test54()
{
    int a;
    int b;
    a = 10;
    b = 20;
    return (a + b) * 3;
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
    return i;
}
int test64()
{
    int i;
    i = 0;
    for (; i < 10;)
        i = i + 1;
    return i;
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
    return i;
}
int test66()
{
    int i;
    i = 0;
    while (i < 10)
        i = i + 1;
    return i;
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
    return i;
}
int test72() { return foo(); }
int foo() { return 10; }
int two(int x) { return 2 * x; }
int test73() { return two(3); }
int test74() { return two(24); }
int test75() { return two(5 * 20 - 2 * (6 + 7)); }
int BIG(int x) { return x + 10; }
int test76() { return BIG(33); }
int test77() { return add(12, 13); }
int add(int x, int y) { return x + y; }
int test78() { return add_tri(4, 50, 14); }
int add_tri(int a, int b, int c) { return a + b + c; }
int test79() { return add_qua(1, 3, 2, 4); }
int add_qua(int a, int b, int c, int d) { return a + b + c + d; }
int test80() { return add_pen(-100, 200, 57, -117, -23); }
int add_pen(int i, int j, int k, int l, int m) { return i + j + k + l + m; }
int test81() { return add_hex(23, 19, -9, -15, 5, 0); }
int add_hex(int one, int two, int three, int four, int five, int six) { return one + two + three + four + five + six; }
int test82() { return hoge(); }
int hoge() { return 30; }
int test83() { return fibo(1); }
int fibo(int index)
{
    if (index == 0)
        return 0;
    if (index == 1)
        return 1;
    return fibo(index - 1) + fibo(index - 2);
}
int test84() { return fibo(5); }
int test85() { return fibo(8); }
int test86() { return fibo(10); }
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
    *(&a - 1) = b;
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
