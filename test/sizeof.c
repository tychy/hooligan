int main()
{
    int a;
    int x;
    int *y;
    int b[10];
    if (sizeof(x) != 4)
        return 1;
    if (sizeof(y) != 8)
        return 1;
    if (sizeof(x + 3) != 4)
        return 1;
    if (sizeof(y + 3) != 8)
        return 1;
    if (sizeof(*y) != 4)
        return 1;
    if (sizeof(1) != 4)
        return 1;
    if (sizeof(sizeof(2)) != 4)
        return 1;
    if (sizeof(b) != 40)
        return 1;
    return 0;
}
