int impl_add()
{
    static int x;
    x += 5;
    return x;
}

int testStaticImplicit()
{
    int res = impl_add();
    if (res != 5)
    {
        return 1;
    }
    res = impl_add();
    if (res != 10)
    {
        return 1;
    }
    return 0;
}

/*
int expl_add()
{
    static int x = 5;
    x += 5;
    return x;
}

int testStaticExplicit()
{
    int res = expl_add();
    if (res != 10)
    {
        return 1;
    }
    res = expl_add();
    if (res != 15)
    {
        return 1;
    }
    return 0;
}

*/
int impl_if()
{
    int hoge = 3;
    if (hoge == 3)
    {
        static int x;
        x += 5;
        hoge += x;
    }
    return hoge;
}

int testStaticScope()
{
    int res = impl_if();
    if (res != 8)
    {
        return 1;
    }
    res = impl_if();
    if (res != 13)
    {
        return 1;
    }
    return 0;
}

int main()
{

    if (testStaticImplicit() != 0)
    {
        return 1;
    }

    if (testStaticScope() != 0)
    {
        return 1;
    }

    /*
    if (testStaticExplicit() != 0)
    {
        return 1;
    }
    */
    return 0;
}