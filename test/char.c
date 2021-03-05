int s;
int main()
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