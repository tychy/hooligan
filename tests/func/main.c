int testFunc()
{
    int x = 10;
    if (11 - x != 1)
    {
        return 1;
    }
    return 0;
}
int sums(int x, int y)
{
    return x + y;
}
int testFuncTooMany()
{
    int x = 10;
    int y = 22;
    if (sums(x, y) != 32)
    {
        return 1;
    }

    if (sums(x, y, y) != 32)
    {
        return 1;
    }
    return 0;
}
int a;
int g[10];

int foo(int *arr)
{
    return arr[2];
}

int *bar(int *b)
{
    return b;
}

int testFuncPtr()
{
    int *p;
    a = 11;
    p = bar(&a);
    if (*p != 11)
    {
        return 1;
    }
    if (*(bar(&a)) != 11)
    {
        return 1;
    }
    g[0] = 1;
    g[1] = 2;
    g[2] = 3;
    if (foo(g) != 3)
    {
        return 1;
    }
    return 0;
}
int foovoid(void)
{
    return 11;
}
int testFuncArgVoid()
{
    if (foovoid() != 11)
    {
        return 1;
    }
    return 0;
}
int z;
void setz()
{
    z = 20;
    return;
}
int testFuncVoid()
{
    setz();
    if (z != 20)
    {
        return 1;
    }
    return 0;
}
typedef struct Hoge Hoge;
struct Hoge
{
    int val;
};

int func_hoge(Hoge h[10])
{
    return h[0].val + h[1].val + h[2].val;
}

int func_array(int c[10])
{
    return c[0] + c[1];
}

int testFuncArray()
{
    int a[10];
    a[0] = 1;
    a[1] = 2;

    if (func_array(a) != 3)
    {
        return 1;
    }
    return 0;
}

int testFuncStructArrayLocal()
{
    Hoge hogearray[10];
    Hoge ha;
    Hoge hb;
    Hoge hc;
    ha.val = 1;
    hb.val = 2;
    hc.val = 3;
    hogearray[0] = ha;
    hogearray[1] = hb;
    hogearray[2] = hc;
    if (func_hoge(hogearray) != 6)
    {
        return 1;
    }
    return 0;
}

int main()
{
    if (testFunc() != 0)
    {
        return 1;
    }
    if (testFuncTooMany() != 0)
    {
        return 1;
    }

    if (testFuncPtr() != 0)
    {
        return 1;
    }
    if (testFuncArgVoid() != 0)
    {
        return 1;
    }
    if (testFuncArray() != 0)
    {
        return 1;
    }

    if (testFuncStructArrayLocal() != 0)
    {
        return 1;
    }

    return 0;
}
