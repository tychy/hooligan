int testAndOperator()
{
    if (!(1 && 1))
    {
        return 1;
    }

    if (!(2 && 1))
    {
        return 1;
    }

    if (!(3 && 1))
    {
        return 1;
    }

    return 0;
}

int main()
{
    int x = 30;
    if (x > 3 && x < 20)
    {
        return 1;
    }

    x = 10;
    if (!(x == 10))
    {
        return 1;
    }
    x = 2;
    if ((x > 3) && (x < 20))
    {
        return 1;
    }

    if (testAndOperator() != 0)
    {
        return 1;
    }
    return 0;
}
