int main()
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