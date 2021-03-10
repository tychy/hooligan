int main()
{
    struct
    {
        int val;
        int offset;
    } Hello;
    Hello.val = 10;
    Hello.offset = 32;
    if (Hello.val != 10)
    {
        return 1;
    }

    if (Hello.offset != 32)
    {
        return 1;
    }
    return 0;
}