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
    for (i = 0; i < 10; i++)
    {
        switch (i)
        {
        case 0:
        case 5:
            continue;
        case 9:
            break;
        }
        printf("%d\n", i);
    }
    if (i != 10)
    {
        return 1;
    }
    return 0;
}
int main()
{
    /*
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
*/
    if (testSwitchCaseMidA() != 0)
    {
        return 1;
    }
    return 0;
}