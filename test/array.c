int main()
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