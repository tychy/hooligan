int testSimpleEnum()
{
    typedef enum
    {
        APPLE,
        ORANGE,
        PEACH,
        LEMON,
    } Fruit;
    Fruit f_a;
    f_a = APPLE;
    if (!(f_a == APPLE && f_a == 0))
    {
        return 1;
    }
    Fruit f_b = 1;
    if (!(f_b == ORANGE && f_b == 1))
    {
        return 1;
    }
    Fruit f_c = PEACH;
    if (!(f_c == PEACH && f_c == 2))
    {
        return 1;
    }
    Fruit f_d = LEMON;
    if (!(f_d == LEMON && f_d == 3))
    {
        return 1;
    }
    return 0;
}

int main()
{
    if (testSimpleEnum() != 0)
    {
        return 1;
    }
    return 0;
}
