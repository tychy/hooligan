int main()
{
    int a;

    a = 0;
    for (int counter = 0; counter < 100; counter = counter + 1)
        if (counter * counter == 400)
            a = counter;
    if (a != 20)
        return 1;

    a = 0;
    int i;
    i = 0;
    while (i < 10)
        if (i == 5)
        {
            a = i;
            i = i + 1;
        }
        else
            i = i + 1;
    if (a != 5)
        return 1;
    return 0;
}
