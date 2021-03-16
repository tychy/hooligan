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
    return 0;
}