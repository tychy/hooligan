int a;
int g[10];

/*comment
     comment */
int foo(int *arr)
{

    /*comment
     comment */
    return arr[2];
}

int *bar(int *b)
{
    //comment

    return b;
}
//comment

int main()
{
    /* comment */
    int *p;
    a = 11;

    p = bar(&a);
    //comment
    if (*p != 11)
    {
        //comment
        //comment

        /* comment */
        // return hogehoge /*
        return 1; // comment
    }
    g[0] = 1;
    //comment

    g[1] = 2;

    /*comment
     comment */
    g[2] = 3;
    if (foo(g) != 3)
    {
        return 1;
    }
    return 0;
}