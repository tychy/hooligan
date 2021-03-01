int main()
{
    int a = 10001;
    int b = 10002;
    int c = 20001;
    int d = 20002;
    int e = 40000;

    int *p;
    int *q;
    p = &a;
    if (*p != 10001)
        return 1;
    q = p - 2;
    if (*q != 20001)
        return 1;
    q = p - 3;
    if (*q != 20002)
        return 1;
    if (*(q - 1) != 40000)
        return 1;
    if (*(q + 2) != 10002)
        return 1;
    if (*(p - 1) + *(p - 2) + *(p - 3) != 50005)
        return 1;
    return 0;
}
