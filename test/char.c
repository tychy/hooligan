int s;
int testCharGvar()
{
    char x[3];
    char y;
    char z;
    int a;
    int b;

    y = 5;
    z = -2;
    x[0] = -1;
    x[1] = 2;
    a = 11;
    b = 2;
    s = -3;
    if (x[0] != -1)
    {
        return 1;
    }
    if (y != 5)
    {
        return 1;
    }

    if (a + y != 16)
    {
        return 1;
    }

    if (z + b != 0)
    {
        return 1;
    }

    if (z - b != -4)
    {
        return 1;
    }

    if (s - x[0] != -2)
    {
        return 1;
    }
    return 0;
}

int testChar()
{
    char x[3];
    char y;
    char z;
    char w;
    int a;
    x[0] = -1;
    y = 1;
    z = 127;
    if (x[0] != -1)
    {
        return 1;
    }
    a = z + 1;
    if (z + 1 != 128)
    {
        return 1;
    }

    if (1 + z != 128)
    {
        return 1;
    }
    if (a != 128)
    {
        return 1;
    }
    w = z + y;
    if (w != -128)
    {
        return 1;
    }
    return 0;
}

int testStringEmpty()
{
    char *x;
    x = "";
    if (x[0] != 0)
    {
        return 1;
    }
    return 0;
}

int testString()
{
    char *y;
    char *z;
    char *v;

    y = "hellllo";
    v = "omg";
    z = y;

    if (y[0] != 104)
        return 1;
    if (y[1] != 101)
        return 1;
    if (y[2] != 108)
        return 1;
    if (y[3] != 108)
        return 1;
    if (y[4] != 108)
        return 1;
    if (y[5] != 108)
        return 1;
    if (y[6] != 111)
        return 1;
    if (z[0] != 104)
        return 1;
    if (z[1] != 101)
        return 1;
    if (z[2] != 108)
        return 1;
    if (z[3] != 108)
        return 1;
    if (z[4] != 108)
        return 1;
    if (z[5] != 108)
        return 1;
    if (z[6] != 111)
        return 1;

    return 0;
}

int main()
{
    if (testChar() != 0)
    {
        return 1;
    }
    if (testCharGvar() != 0)
    {
        return 1;
    }
    if (testStringEmpty() != 0)
    {
        return 1;
    }
    if (testString() != 0)
    {
        return 1;
    }
    return 0;
}