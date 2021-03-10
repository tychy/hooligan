
int teststruct()
{
    struct
    {
        int val;
        int offset;
    } Hello;
    Hello.val = 10;
    Hello.offset = 32;
    if (Hello.val != 10)
    {
        return 1;
    }

    return 10 + Hello.val;
}

int teststructarray()
{
    struct
    {
        int val;
        int offset;
        int a[10];
    } b;
    b.val = 12;
    b.offset = 2;
    for (int i = 0; i < 10; i = i + 1)
    {
        b.a[i] = i + 1;
    }

    int res = 0;
    for (int i = 0; i < 10; i = i + 1)
    {
        res = res + b.a[i];
    }
    return res;
}
int main()
{
    if (20 != teststruct())
    {
        return 1;
    }
    if (55 != teststructarray())
    {
        return 1;
    }
    return 0;
}