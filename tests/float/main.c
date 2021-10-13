int testFloatDefine()
{
    float x;
    float y = 1.0;
    x = 1.1;
    return 0;
}

int testFloatEq()
{
    float x = 1.0;
    float y = 1.0555;
    float z = 1.0;
    float ze = 0.0;

    if (x == 0.0)
    {
        return 1;
    }
    if (x == 5.0)
    {
        return 1;
    }
    if (!(x == 1.0))
    {
        return 1;
    }

    if (y == 1.05551)
    {
        return 1;
    }
    if (!(x == z))
    {
        return 1;
    }
    if (!(ze == 0.00))
    {
        return 1;
    }
    return 0;
}

int testFloatNEq()
{
    float x = 1.0;
    float y = 2.0;
    float z = 1.0;
    float ze = 0.0;
    float zee = 0.000;
    if (ze != 0.0)
    {
        return 1;
    }
    if (zee != 0.0)
    {
        return 1;
    }
    if (zee != 0.00000)
    {
        return 1;
    }
    if (!(ze != x))
    {
        return 1;
    }

    if (x != 1.0)
    {
        return 1;
    }
    if (!(x != 1.01))
    {
        return 1;
    }
    if (x != z)
    {
        return 1;
    }
    return 0;
}

int testFloatGEq()
{
    float x = 1.0;
    float y = 2.0;
    float z = 3.0;
    float ze = 0.0;
    float zee = 0.000;

    float a = 1.11;
    float b = 1.112;
    float c = 1.11;

    if (x >= y)
    {
        return 1;
    }

    if (y >= z)
    {
        return 1;
    }

    if (ze >= z)
    {
        return 1;
    }

    if (a >= b)
    {
        return 1;
    }
    if (!(a >= c))
    {
        return 1;
    }

    return 0;
}

int testFloatLEq()
{
    float x = 1.0;
    float y = 2.0;
    float z = 3.0;
    float ze = 0.0;
    float zee = 0.000;

    float a = 1.11;
    float b = 1.112;
    float c = 1.11;

    if (y <= x)
    {
        return 1;
    }

    if (z <= y)
    {
        return 1;
    }

    if (z <= ze)
    {
        return 1;
    }

    if (b <= a)
    {
        return 1;
    }

    if (!(a <= c))
    {
        return 1;
    }

    return 0;
}

int testFloatG()
{
    float x = 1.0;
    float y = 2.0;
    float z = 3.0;
    float ze = 0.0;
    float zee = 0.000;

    float a = 1.11;
    float b = 1.112;
    if (x > y)
    {
        return 1;
    }

    if (y > z)
    {
        return 1;
    }

    if (ze > z)
    {
        return 1;
    }

    if (a > b)
    {
        return 1;
    }

    return 0;
}

int testFloatL()
{
    float x = 1.0;
    float y = 2.0;
    float z = 3.0;
    float ze = 0.0;
    float zee = 0.000;

    float a = 1.11;
    float b = 1.112;

    if (y < x)
    {
        return 1;
    }

    if (z < y)
    {
        return 1;
    }

    if (z < ze)
    {
        return 1;
    }

    if (b < a)
    {
        return 1;
    }

    return 0;
}

int main()
{
    if (testFloatDefine())
    {
        return 1;
    }

    if (testFloatEq())
    {
        return 1;
    }

    if (testFloatNEq())
    {
        return 1;
    }

    if (testFloatG())
    {
        return 1;
    }

    if (testFloatL())
    {
        return 1;
    }
    return 0;
}