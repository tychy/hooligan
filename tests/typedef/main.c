typedef int INTEGER;
int testTypedefGlob()
{
    INTEGER twenty = 20;
    if (twenty != 20)
    {
        return 1;
    }
    return 0;
}

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

int testTypedefLocGlob()
{
    typedef struct
    {
        int a;
        int b;
    } INTEGER;
    INTEGER integer;
    integer.a = 10;
    integer.b = 100;
    if (integer.a != 10)
    {
        return 1;
    }
    if (integer.b != 100)
    {
        return 1;
    }
    return 0;
}

int main()
{
    if (testTypedefGlob() != 0)
    {
        return 1;
    }
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
    if (testTypedefLocGlob() != 0)
    {
        return 1;
    }
    return 0;
}
