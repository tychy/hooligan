static int x;
static int y = 11;
static int func_b()
{
    x = 10;
    return x;
}

int func_c()
{
    return func_b();
}

int func_d()
{
    return y + 11;
}