int testLine()
{
    if (__LINE__ != 3)
    {
        return 1;
    }

    if (__LINE__ != 8)
    {
        return 1;
    }

    return 0;
}

int testLineWithDirective()
{
#line 3
    if (__LINE__ != 3)
    {
        return 1;
    }

    if (__LINE__ != 8)
    {
        return 1;
    }

    return 0;
}

int main()
{
    if (testLine() != 0)
    {
        return 1;
    }
    if (testLineWithDirective() != 0)
    {
        return 1;
    }

    return 0;
}