
int teststruct()
{
    struct
    {
        int val;
        int offset;
    } Hello;
    Hello.val = 10;
    if (Hello.val != 10)
    {
        return 1;
    }

    return 10 + Hello.val;
}

int main()
{
    return teststruct();
}