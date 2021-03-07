int main()
{
    char *y;
    char *z;
    y = "hellllo";
    z = y;
    if (y[0] != 104)
        return 1;
    if (y[1] != 101)
        return 1;
    if (y[2] != 108)
        return 1;
    if (y[3] != 108)
        return 1;
    if (y[4] != 108)
        return 1;
    if (y[5] != 108)
        return 1;
    if (y[6] != 111)
        return 1;
    if (z[0] != 104)
        return 1;
    if (z[1] != 101)
        return 1;
    if (z[2] != 108)
        return 1;
    if (z[3] != 108)
        return 1;
    if (z[4] != 108)
        return 1;
    if (z[5] != 108)
        return 1;
    if (z[6] != 111)
        return 1;

    return 0;
}