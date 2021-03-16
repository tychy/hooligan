int testSwitchCaseEasyA()
{
    int x = 5;
    switch (x)
    {
    case 5:
        return 0;
    case 6:
        return 1;
    case 10:
    case 11:
        return 1;
    case 111:
    default:
        return 1;
    }
    return 1;
}
int testSwitchCaseEasyB()
{
    int x = 6;
    switch (x)
    {
    case 5:
        return 1;

    case 6:
        return 0;
    case 10:
    case 11:
        return 1;
    case 111:

    default:
        return 1;
    }
    return 1;
}
int testSwitchCaseEasyC()
{
    int x = 11;
    switch (x)
    {
    case 5:
        return 1;

    case 6:
        return 1;
    case 10:
    case 11:
        return 0;
    case 111:
    default:
        return 1;
    }
    return 1;
}
int testSwitchCaseEasyD()
{
    int x = 200;
    switch (x)
    {
    case 5:
        return 1;
    case 6:
        return 1;
    case 10:
    case 11:
        return 1;
    case 111:
    default:
        return 0;
    }
    return 1;
}

int testSwitchCaseEasyE()
{
    int x = 111;
    switch (x)
    {
    case 5:
        return 1;
    case 6:
        return 1;
    case 10:
    case 11:
        return 1;
    case 111:
    default:
        return 0;
    }
    return 1;
}

int testSwitchCaseMidA()
{
    int i;
    int sums = 0;
    for (i = 0; i < 10; i++)
    {
        switch (i)
        {
        case 0:
        case 5:
        case 9:
            sums += 1;
            break;
        case 10:
            sums = 0;
        }
    }
    if (sums != 3)
    {
        return 1;
    }
    return 0;
}
int testSwitchCaseMidB()
{
    int i;
    int sums = 0;
    for (i = 0; i < 11; i++)
    {
        switch (i)
        {
        case 0:
        case 5:
        case 9:
            sums += 1;
            break;
        case 10:
            sums = 100;
        }
    }
    if (sums != 100)
    {
        return 1;
    }
    return 0;
}

int testSwitchCaseMidC()
{
    int i = 7;
    int x = 11;
    int res = 0;
    switch (i)
    {
    case 0:
        res = 0;
        break;
    case 5:
        res = 10;
        res = 5;
        break;
    case 7:
        res = 10;
        res = 7;
        break;
    case 9:
        res = 9;
        break;
    case 10:
        res = 10;
        break;
    }
    if (res != 7)
    {
        return 1;
    }
    return 0;
}

int main()
{
    if (testSwitchCaseEasyA() != 0)
    {
        return 1;
    }

    if (testSwitchCaseEasyB() != 0)
    {
        return 1;
    }

    if (testSwitchCaseEasyC() != 0)
    {
        return 1;
    }

    if (testSwitchCaseEasyD() != 0)
    {
        return 1;
    }

    if (testSwitchCaseEasyE() != 0)
    {
        return 1;
    }

    if (testSwitchCaseMidA() != 0)
    {
        return 1;
    }

    if (testSwitchCaseMidB() != 0)
    {
        return 1;
    }

    if (testSwitchCaseMidC() != 0)
    {
        return 1;
    }

    return 0;
}