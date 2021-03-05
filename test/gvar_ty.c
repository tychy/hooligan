int *a;
int gvar;
int main()
{
    int b;
    int *c;
    b = 3;
    a = &b;
    if (*a != 3)
    {
        return 1;
    }
    *a = 5;
    if (b != 5)
    {
        return 1;
    }
    gvar = 11;
    c = &gvar;
    if (*c != 11)
    {
        return 1;
    }
    return 0;
}