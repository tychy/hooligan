int a[10];
int b;

int main()
{
    int i;
    for (i = 0; i < 10; i = i + 1)
    {
        a[i] = i + 1;
    }
    int sums;
    sums = 0;
    for (i = 0; i < 10; i = i + 1)
    {
        sums = sums + a[i];
    }
    if (sums != 55)
    {
        return 1;
    }
    return 0;
}