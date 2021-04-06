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
int testExternHelp()
{

    extern int e_two;
    e_one = 1111;
    e_two = 2222;
    return 0;
}
int e_one;
int e_two;
int testExtern()
{
    // 勝手に０で初期化されているっぽい？
    if (e_one != 0)
    {
        return 1;
    }
    if (e_two != 0)
    {
        return 1;
    }
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
int xx = 5;
int testGlobalInit()
{
    if (xx != 5)
    {
        return 1;
    }

    return 0;
}

int xarr[3] = {1, 2, 3};
int yarr[5] = {1, 2, 3};
int zarr[2] = {1, 2, 3};
int testGlobalInitArray()
{
    if (xarr[0] != 1)
    {
        return 1;
    }

    if (yarr[1] != 2)
    {
        return 1;
    }

    if (yarr[3] != 0)
    {
        return 1;
    }

    if (zarr[1] != 2)
    {
        return 1;
    }
    return 0;
}

char gvar_x = 'x';
char *gvar_y = "abc";
int testGlobalInitConst()
{
    if (gvar_x != 'x')
    {
        return 1;
    }

    if (gvar_y[0] != 'a')
    {
        return 1;
    }
    return 0;
}
char *operator_list[2] = {"++", "+="};

int testGlobalInitString()
{
    if (operator_list[0][0] != '+')
    {
        return 1;
    }
    if (operator_list[0][1] != '+')
    {
        return 1;
    }
    if (operator_list[1][0] != '+')
    {
        return 1;
    }
    if (operator_list[1][1] != '=')
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

    if (testGlobalInit() != 0)
    {
        return 1;
    }
    if (testGlobalInitArray() != 0)
    {
        return 1;
    }
    if (testGlobalInitConst() != 0)
    {
        return 1;
    }

    if (testGlobalInitString() != 0)
    {
        return 1;
    }
    return 0;
}
