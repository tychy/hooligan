int testConstInt()
{
    const int x = 10;
    if (x != 10)
    {
        return 1;
    }
    return 0;
}

int main()
{
    if (testConstInt() != 0)
    {
        return 1;
    }
    return 0;
}