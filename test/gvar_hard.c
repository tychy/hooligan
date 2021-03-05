int gvar;
int a;
int foo(int index)
{
    gvar = index;
    return index;
}
int main()
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
