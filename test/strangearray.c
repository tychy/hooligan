int main()
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