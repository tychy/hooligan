int testArrayInit()
{
    int x[3] = {1, 2, 3};
    int y[4] = {1, 2, 3};
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
    if (y[0] != 1)
    {
        return 1;
    }
    if (y[1] != 2)
    {
        return 1;
    }
    if (y[2] != 3)
    {
        return 1;
    }
    if (y[3] != 0)
    {
        return 1;
    }
    return 0;
}

int testArrayInitChar()
{
    char c_one[4] = {'a', 'b', 'c', 0};
    if (c_one[0] != 'a')
    {
        return 1;
    }
    if (c_one[1] != 'b')
    {
        return 1;
    }
    if (c_one[2] != 'c')
    {
        return 1;
    }
    printf(c_one);
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

int testArrayCharPtr()
{
    char *strings[4] = {
        "Python",
        "Ruby",
        "PHP",
        "Swift",
    };
    for (int i = 0; i < 4; i += 1)
    {
        printf(strings[i]);
    }
    return 0;
}

int testArrayNonSpecifyCount()
{
    int a[] = {10, 11, 12};
    int b[3] = {10, 11, 12};
    if (sizeof(a) != sizeof(b))
    {
        return 1;
    }
    for (int i = 0; i < 3; i += 1)
    {
        if (a[i] != b[i])
        {
            return 1;
        }
    }
    int c[] = {'h', 'e', 'l', 'l', 'o', 0};
    int d[6] = {'h', 'e', 'l', 'l', 'o', 0};
    if (sizeof(c) != sizeof(d))
    {
        return 1;
    }
    int cnt = 0;
    for (; cnt < sizeof(d) / sizeof(d[0]); cnt += 1)
    {
        if (c[cnt] != d[cnt])
        {
            return 1;
        }
    }
    if (cnt != 6)
    {
        return 1;
    }
    return 0;
}

int testArrayInitString()
{
    char c_a[] = "java";
    char c_b[7] = "kotlin";
    char c_over[3] = "csharp"; // 現状の実装では配列のサイズを上書きしてしまう
    printf(c_a);
    printf(c_b);
    printf(c_over);
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

    if (testArrayInitChar() != 0)
    {
        return 1;
    }

    if (testArrayCharPtr() != 0)
    {
        return 1;
    }

    if (testArrayHard() != 0)
    {
        return 1;
    }

    if (testArrayNonSpecifyCount() != 0)
    {
        return 1;
    }

    if (testArrayInitString() != 0)
    {
        return 1;
    }
    return 0;
}
