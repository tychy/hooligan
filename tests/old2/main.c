int test92()
{
    int p;
    int q;
    int *r;
    p = 10;
    q = 20;
    r = &q;
    return *(r + 1);
}
int test93()
{
    int p;
    int q;
    p = 100;
    q = 20;
    return *(&q + 1);
}

int main()
{
    if (test92() != 10)
    {
        return 1;
    }
    if (test93() != 100)
    {
        return 1;
    }
    return 0;
}
