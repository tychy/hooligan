int testArrayInit()
{
    int x[3] = {1, 2, 3};
    if (x[0] != 1)
    {
        return 1;
    }
    if (x[1] != 2)
    {
        return 1;
    }
    if (x[2] != 3)
    {
        return 1;
    }
    return 0;
}

int testArray()
{
    int a[4];
    *a = 4;
    *(a + 1) = 2;
    *(a + 2) = 5;
    *(a + 3) = 15;

    if (*a != 4)
    {
        return 1;
    }

    if (*(a + 2) != 5)
    {
        return 1;
    }
    int b;
    int *c;
    b = *a;
    c = a;
    if (b != 4)
    {
        return 1;
    }
    if (*c != 4)
    {
        return 1;
    }
    if (a[0] != 4)
    {
        return 1;
    }

    if (a[1] != 2)
    {
        return 1;
    }

    if (a[2] != 5)
    {
        return 1;
    }
    a[2] = 8;

    if (a[2] != 8)
    {
        return 1;
    }
    if (a[3] != 15)
    {
        return 1;
    }
    return 0;
}

int testArrayHard()
{
    int a[10];
    int i;
    for (i = 0; i < 10; i = i + 1)
    {
        a[i] = i + 1;
    }
    int sums;
    sums = 0;
    for (i = 0; i < 10; i = i + 1)
    {
        sums = sums + a[i];
    }
    if (sums != 55)
    {
        return 1;
    }
    if (3 [a] != a[3])
    {
        return 1;
    }
    if ((3 + 2)[a] != a[5])
    {
        return 1;
    }
    if (3 [(a + 1)] != a[4])
    {
        return 1;
    }
    a[0] = -3;
    if (a[0] != -3)
    {
        return 1;
    }
    return 0;
}
int main()
{
    if (testArray() != 0)
    {
        return 1;
    }

    if (testArrayInit() != 0)
    {
        return 1;
    }

    if (testArrayHard() != 0)
    {
        return 1;
    }
    return 0;
}