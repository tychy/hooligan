int main()
{
    typedef struct Left Left;
    typedef struct Right Right;
    struct Left
    {
        Right *rev;
        int val;
    };
    struct Right
    {
        Left *rev;
        int val;
    };
    Left l;
    Right r;
    l.val = 10;
    r.val = 20;
    l.rev = &r;
    r.rev = &l;
    if (l.val != 10)
    {
        return 1;
    }
    if (r.val != 20)
    {
        return 1;
    }
    if (l.rev->val != 20)
    {
        return 1;
    }
    if (l.rev->rev->val != 10)
    {
        return 1;
    }
    if (r.rev->val != 10)
    {
        return 1;
    }
    if (r.rev->rev->val != 20)
    {
        return 1;
    }
    return 0;
}
