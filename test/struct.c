
int main()
{
    struct
    {
        int val;
        int offset;
    } Hello;
    Hello.val = 10;
    Hello.offset = 32;
    int x;
    x = Hello.val;
    if (Hello.val != 10)
    {
        return Hello.val;
    }
    if (x != 10)
    {
        return x;
    }

    if (Hello.offset != 32)
    {
        return 1;
    }
    return 0;
}