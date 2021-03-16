
int func_a()
{
    return 'a';
}

static int func_b()
{
    return 111;
}
int func_d()
{
    return func_b();
}
int main()
{
    func_a();
    if (func_b() != 111)
    {
        return 1;
    }

    if (func_c() != 10)
    {
        return 1;
    }

    if (func_d() != 111)
    {
        return 1;
    }

    return 0;
}
