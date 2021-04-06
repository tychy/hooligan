int fibo(int index)
{
    if (index == 0)
        return 0;
    if (index == 1)
        return 1;
    return fibo(index - 1) + fibo(index - 2);
}
int testFibo()
{
    if (fibo(0) != 0)
        return 1;
    if (fibo(1) != 1)
        return 1;
    if (fibo(2) != 1)
        return 1;
    if (fibo(3) != 2)
        return 1;
    if (fibo(4) != 3)
        return 1;
    if (fibo(5) != 5)
        return 1;
    if (fibo(6) != 8)
        return 1;
    if (fibo(7) != 13)
        return 1;
    if (fibo(8) != 21)
        return 1;
    if (fibo(9) != 34)
        return 1;
    if (fibo(10) != 55)
        return 1;
    if (fibo(11) != 89)
        return 1;
    return 0;
}

int testMinus()
{
    int x = -5;
    if (x != -5)
    {
        return 1;
    }
    return 0;
}

int testCalc()
{
    int a;
    a = 1 + 2 + 3;
    if (a != 6)
        return 1;
    a = (4 * 9) - (6 * 5) / 7;
    if (a == 32)
        a = 0;
    else
        return 1;
    a = 4 * 5 * 2 + 6 * 7 - 5 * 9;
    if (a != 37)
        return 1;

    a = (4 > 1) > (1 < 0);
    if (a != 1)
        return 1;
    a -= 41;
    if (a != -40)
    {
        return 1;
    }

    int b = 11;
    int c = 128;
    if (b % 2 != 1)
    {
        return 1;
    }

    if (b % 3 != 2)
    {
        return 1;
    }

    if (c % 2 != 0)
    {
        return 1;
    }

    if (c % 10 != 8)
    {
        return 1;
    }
    b *= 2;
    c *= 3;
    if (b != 22)
    {
        return 1;
    }

    if (c != 384)
    {
        return 1;
    }

    c %= 2;
    if (c != 0)
    {
        return 1;
    }
    return 0;
}

int testPtr()
{
    int a = 10001;
    int b = 10002;
    int c = 20001;
    int d = 20002;
    int e = 40000;

    int *p;
    int *q;
    p = &a;
    if (*p != 10001)
        return 1;
    q = p - 2;
    if (*q != 20001)
        return 1;
    q = p - 3;
    if (*q != 20002)
        return 1;
    if (*(q - 1) != 40000)
        return 1;
    if (*(q + 2) != 10002)
        return 1;
    if (*(p - 1) + *(p - 2) + *(p - 3) != 50005)
        return 1;
    return 0;
}

int testSubPtr()
{
    char *x = "abcdef";
    char *pos;
    pos = x;
    pos++;
    if (pos - x != 1)
    {
        return 1;
    }
    pos++;
    if (pos - x != 2)
    {
        return 1;
    }

    return 0;
}

int main()
{

    if (testFibo() != 0)
    {
        return 1;
    }
    if (testMinus() != 0)
    {
        return 1;
    }
    if (testCalc() != 0)
    {
        return 1;
    }
    if (testPtr() != 0)
    {
        return 1;
    }
    if (testSubPtr() != 0)
    {
        return 1;
    }
    return 0;
}
