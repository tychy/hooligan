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

int testStructTag()
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
int main()
{
    if (testStruct() != 0)
    {
        return 1;
    }
    if (testStructTag() != 0)
    {
        return 1;
    }
    return 0;
}