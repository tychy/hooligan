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

    // 短絡評価のテスト
    typedef struct hoge hoge;
    struct hoge
    {
        int a;
        int b;
    };
    hoge *h = 0;
    if (h && h->a)
    {
        return 1;
    }
    if (!(1 || h->b))
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
