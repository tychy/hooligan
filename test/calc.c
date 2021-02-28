int main()
{
    int a;
    a = 1 + 2 + 3;
    if (a != 6)
        return 1;
    a = (4 * 9) - (6 * 5) / 7;
    if (a == 32)
        a = 0;
    else
        return 1;
    a = 4 * 5 * 2 + 6 * 7 - 5 * 9;
    if (a != 37)
        return 1;

    a = (4 > 1) > (1 < 0);
    if (a != 1) 
        return 1; 

    return 0;
}
