int testStructsCircularRefLocal()
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
typedef struct Up Up;
typedef struct Down Down;

struct Up
{
    Down *rev;
    int val;
};

struct Down
{
    Up *rev;
    int val;
};

int testStructsCircularRefGlobal()
{
    Up u;
    Down d;
    u.val = 10;
    d.val = 20;
    u.rev = &d;
    d.rev = &u;
    if (u.val != 10)
    {
        return 1;
    }
    if (d.val != 20)
    {
        return 1;
    }
    if (u.rev->val != 20)
    {
        return 1;
    }
    if (u.rev->rev->val != 10)
    {
        return 1;
    }
    if (d.rev->val != 10)
    {
        return 1;
    }
    if (d.rev->rev->val != 20)
    {
        return 1;
    }

    return 0;
}
int main()
{
    if (testStructsCircularRefLocal() != 0)
    {
        return 1;
    }
    if (testStructsCircularRefGlobal() != 0)
    {
        return 1;
    }
    return 0;
}
