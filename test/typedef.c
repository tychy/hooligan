int testTypedefInt()
{
    typedef int hoge;
    hoge x = 3;
    if (x != 3)
    {
        return 1;
    }
    return 0;
}

int testTypedefChar()
{
    typedef char foo;
    foo y = 8;
    foo z = 15;

    if (y != 8)
    {
        return 1;
    }

    if (z - y != 7)
    {
        return 1;
    }
    return 0;
}

int testTypedefStrust()
{
    typedef struct
    {
        int x;
        int y;
    } Hello;
    Hello a;
    a.x = 3;
    a.y = 11;
    if (a.x * a.y != 33)
    {
        return 1;
    }
    return 0;
}

int main()
{
    if (testTypedefInt() != 0)
    {
        return 1;
    }

    if (testTypedefChar() != 0)
    {
        return 1;
    }

    if (testTypedefStrust() != 0)
    {
        return 1;
    }
    return 0;
}
