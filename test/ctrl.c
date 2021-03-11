int testCtrl()
{
    int a;

    a = 0;
    for (int counter = 0; counter < 100; counter = counter + 1)
        if (counter * counter == 400)
            a = counter;
    if (a != 20)
        return 1;

    a = 0;
    int i;
    i = 0;
    while (i < 10)
        if (i == 5)
        {
            a = i;
            i = i + 1;
        }
        else
            i = i + 1;
    if (a != 5)
        return 1;
    return 0;
}

int testBreak()
{
    int res = -1;
    for (int i = 0; i < 100; i = i + 1)
    {
        if (i == 50)
        {
            res = i;
            break;
        }
    }
    if (res != 50)
    {
        return 1;
    }
    return 0;
}

int testWhileContinue()
{
    int total = 0;
    int i = 0;
    while (i <= 10)
    {
        if (i / 2 != i - i / 2) // 奇数
        {
            i = i + 1;
            continue;
        }
        total = total + i;
        i = i + 1;
    }
    if (total != 30)
    {
        return 1;
    }
    return 0;
}

int main()
{
    if (testCtrl() != 0)
    {
        return 1;
    }
    if (testBreak() != 0)
    {
        return 1;
    }
    if (testWhileContinue() != 0)
    {
        return 1;
    }
    return 0;
}
