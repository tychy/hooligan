int testfunc()
{
    int x = 10;
    return 11 - x;
}
int main()
{
    if (testfunc() != 0)
    {
        return 1;
    };
    return 0;
}