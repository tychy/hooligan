int testFunc()
{
    int x = 10;
    if (11 - x != 1)
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
    g[0] = 1;
    g[1] = 2;
    g[2] = 3;
    if (foo(g) != 3)
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
    if (testFuncPtr() != 0)
    {
        return 1;
    }

    return 0;
}