int testStruct()
{
    struct
    {
        int a;
        int b;
    } x;
    x.a = 10;
    if (x.a != 10)
    {
        return 1;
    }
    return 0;
}

int testStructTagLocal()
{
    struct hoge
    {
        int a;
        int b;
    };
    struct hoge x;
    x.a = 10;
    if (x.a != 10)
    {
        return 1;
    }
    return 0;
}

struct foo
{
    int s;
    int t;
};

int testStructTagGlobal()
{
    struct foo x;
    x.s = 10;
    if (x.s != 10)
    {
        return 1;
    }
    return 0;
}

int main()
{
    if (testStruct() != 0)
    {
        return 1;
    }

    if (testStructTagLocal() != 0)
    {
        return 1;
    }

    if (testStructTagGlobal() != 0)
    {
        return 1;
    }
    return 0;
}