int i;

int test_gvar()
{
    if (i != 10)
    {
        return 1;
    }
    return 0;
}

int main()
{
    i = 10;
    if (i != 10)
    {
        return 1;
    }
    int i = 20;
    if (i != 20)
    {
        return 1;
    }
    int sum = 0;
    for (int i = 0; i < 10; i = i + 1)
    {
        sum = sum + i;
    }
    if (sum != 45)
    {
        return 1;
    }
    if (i != 20)
    {
        return 1;
    }
    {
        int i = 30;
        if (i != 30)
        {
            return 1;
        }
    }
    if (i != 20)
    {
        return 1;
    }
    if (test_gvar() != 0)
    {
        return 1;
    }
    return 0;
}
