int x;
int arr[10];
int b;
int *avar;
int gvarvar;

int testGvarPtr()
{
    int b;
    int *c;
    b = 3;
    avar = &b;
    if (*avar != 3)
    {
        return 1;
    }
    *avar = 5;
    if (b != 5)
    {
        return 1;
    }
    gvarvar = 11;
    c = &gvarvar;
    if (*c != 11)
    {
        return 1;
    }
    return 0;
}
int testGvarArray()
{
    int i;
    for (i = 0; i < 10; i = i + 1)
    {
        arr[i] = i + 1;
    }
    int sums;
    sums = 0;
    for (i = 0; i < 10; i = i + 1)
    {
        sums = sums + arr[i];
    }
    if (sums != 55)
    {
        return 1;
    }
    return 0;
}

int testGvar()
{
    x = 2;
    if (x != 2)
    {
        return 1;
    }

    return 0;
}
int gvar;
int foo(int index)
{
    gvar = index;
    return index;
}
int a;
int testGvarHard()
{
    int b = 7;
    a = 12;
    gvar = 2;
    if (gvar != 2)
    {
        return 1;
    }
    gvar = 5;
    if (gvar != 5)
    {
        return 1;
    }

    if (a != 12)
    {
        return 1;
    }
    if (a + 1 != 13)
    {
        return 1;
    }
    if (gvar + 1 != 6)
    {
        return 1;
    }

    if (gvar + a != 17)
    {
        return 1;
    }

    if (gvar * a != 60)
    {
        return 1;
    }
    gvar = 19;
    if (gvar * b != 7 * 19)
    {
        return 1;
    }
    if (b - a != -5)
    {
        return 1;
    }
    a = foo(13);
    if (gvar != 13)
    {
        return 1;
    }
    if (a != 13)
    {
        return 1;
    }

    return 0;
}

extern int e_one; // この行を消すと"識別子を解決できませんでした"となる
extern int e_two;
int testExternHelp()
{
    e_one = 1111;
    e_two = 2222;
    return 0;
}
int e_one;
int e_two;
int testExtern()
{
    testExternHelp();
    if (e_one != 1111)
    {
        return 1;
    }
    if (e_two != 2222)
    {
        return 1;
    }
    return 0;
}

int main()
{
    if (testGvar() != 0)
    {
        return 1;
    }
    if (testGvarPtr() != 0)
    {
        return 1;
    }
    if (testGvarArray() != 0)
    {
        return 1;
    }
    if (testGvarHard() != 0)
    {
        return 1;
    }
    if (testExtern() != 0)
    {
        return 1;
    }
    return 0;
}
