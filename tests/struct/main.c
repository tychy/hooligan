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
        int xx;
        int yy;
        int zz;
    };
    ma xyz;
    xyz.zz = 10;
    if (xyz.zz != 10)
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
typedef struct money bank;
struct money
{
    int yen;
    int dollar;
    int euro;
};

int testTypedefStructGlobalA()
{
    bank wallet;
    wallet.yen = 1000;
    wallet.dollar = 2;
    wallet.euro = 3;
    if (wallet.yen / 100 + wallet.dollar != 12)
    {
        return 1;
    }
    return 0;
}

typedef struct continent
{
    int africa;
    int eurasia;
    int america;
} c;

int testTypedefStructGlobalB()
{
    c map;
    map.africa = 1;
    map.eurasia = 3;
    map.america = 8;
    if (map.africa + map.america != 9)
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

    if (testTypedefStructGlobalA() != 0)
    {
        return 1;
    }

    if (testTypedefStructGlobalB() != 0)
    {
        return 1;
    }
    return 0;
}