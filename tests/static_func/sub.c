static int x;
static int func_b()
{
    x = 10;
    return x;
}

int func_c()
{
    return func_b();
}
