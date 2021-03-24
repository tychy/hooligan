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

int testTypedefStructLocalA()
{
    typedef struct f ma;
    struct f
    {
        int z;
    };
    ma x;
    x.z = 10;
    if (x.z != 10)
    {
        return 1;
    }
    return 0;
}

int testTypedefStructLocalB()
{
    typedef struct f
    {
        int x;
        int z;
    } ma;
    ma newton;
    newton.x = 11;
    newton.z = 1;
    if (newton.x - newton.z != 10)
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

    if (testTypedefStructLocalA() != 0)
    {
        return 1;
    }

    if (testTypedefStructLocalB() != 0)
    {
        return 1;
    }

    return 0;
}