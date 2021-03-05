int *a;

int main()
{
    int b;
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
    return 0;
}