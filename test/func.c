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
    int x = setz();
    if (z != 20)
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
    return 0;
}
