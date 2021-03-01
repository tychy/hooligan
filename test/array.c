int main()
{
    int a[4];
    *a = 4;
    *(a + 1) = 2;
    if (*a != 4)
    {
        return 1;
    }

    return 0;
}