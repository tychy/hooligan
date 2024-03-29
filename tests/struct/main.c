int testStruct()
{
    struct
    {
        int a;
        int b;
    } x;
    x.a = 10;
    if (x.a != 10)
    {
        return 1;
    }
    return 0;
}

int testStructTagLocal()
{
    struct hoge
    {
        int a;
        int b;
    };
    struct hoge x;
    x.a = 10;
    if (x.a != 10)
    {
        return 1;
    }
    return 0;
}

struct foo
{
    int s;
    int t;
};

int testStructTagGlobal()
{
    struct foo x;
    x.s = 10;
    if (x.s != 10)
    {
        return 1;
    }
    return 0;
}

int testTypedefStructLocalA()
{
    typedef struct f ma;
    struct f
    {
        int xx;
        int yy;
        int zz;
    };
    ma xyz;
    xyz.zz = 10;
    if (xyz.zz != 10)
    {
        return 1;
    }
    return 0;
}

int testTypedefStructLocalB()
{
    typedef struct f
    {
        int x;
        int z;
    } ma;
    ma newton;
    newton.x = 11;
    newton.z = 1;
    if (newton.x - newton.z != 10)
    {
        return 1;
    }
    return 0;
}
typedef struct money bank;
struct money
{
    int yen;
    int dollar;
    int euro;
};

int testTypedefStructGlobalA()
{
    bank wallet;
    wallet.yen = 1000;
    wallet.dollar = 2;
    wallet.euro = 3;
    if (wallet.yen / 100 + wallet.dollar != 12)
    {
        return 1;
    }
    return 0;
}

typedef struct continent
{
    int africa;
    int eurasia;
    int america;
} c;

int testTypedefStructGlobalB()
{
    c map;
    map.africa = 1;
    map.eurasia = 3;
    map.america = 8;
    if (map.africa + map.america != 9)
    {
        return 1;
    }
    return 0;
}

int testTypedefStructPtr()
{
    c *map;
    map = calloc(1, sizeof(c));
    map->africa = 1;
    map->eurasia = 3;
    map->america = 8;
    if (map->africa + map->america != 9)
    {
        return 1;
    }
    return 0;
}

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

typedef struct A A;
typedef struct B B;
struct A
{
    int val;
    B *next;
};

struct B
{
    int val;
    A *next;
};

int testStructsCircularRefGlobalPtr()
{

    A *a = calloc(1, sizeof(A));

    B *b = calloc(1, sizeof(B));
    a->next = b;
    a->val = 5;
    b->next = a;
    b->val = 11;
    if (b->val != 11)
    {
        return 1;
    }
    if (b->next->val != 5)
    {
        return 1;
    }

    return 0;
}

int testTypedefStructSelf()
{
    typedef struct Graph Graph;
    struct Graph
    {
        int val;
        Graph *next;
    };
    Graph *g = calloc(1, sizeof(Graph));
    g->val = 10;

    Graph *f = calloc(1, sizeof(Graph));
    f->val = 20;
    g->next = f;
    f->next = g;

    if (g->next->next->val != 10)
    {
        return 1;
    }
    return 0;
}

typedef struct IntCharMix IntCharMix;
struct IntCharMix
{
    int index;
    char c;
    int val;
    char s;
    int *next;
};

int get_val_mod2(IntCharMix *mix)
{
    return mix->val % 2;
}

char add_c_s(IntCharMix *mix)
{
    return mix->c + mix->s;
}

int testStructAllign()
{
    IntCharMix mix;
    IntCharMix mix_next;
    mix.index = 1;
    mix.val = 11;
    mix.c = 6;
    mix.s = 1;
    mix_next.index = 2;
    mix_next.val = 100;
    mix_next.c = 1;
    mix_next.s = 7;

    mix.next = &mix_next;
    if (mix.c != 6)
    {
        return 1;
    }
    if (get_val_mod2(&mix) != 1)
    {
        return 1;
    }
    if (get_val_mod2(&mix_next) != 0)
    {
        return 1;
    }
    if (get_val_mod2(mix.next) != 0)
    {
        return 1;
    }

    if (add_c_s(&mix) != 7)
    {
        return 1;
    }
    return 0;
}
int main()
{
    if (testStruct() != 0)
    {
        return 1;
    }

    if (testStructTagLocal() != 0)
    {
        return 1;
    }

    if (testStructTagGlobal() != 0)
    {
        return 1;
    }

    if (testTypedefStructLocalA() != 0)
    {
        return 1;
    }

    if (testTypedefStructLocalB() != 0)
    {
        return 1;
    }

    if (testTypedefStructGlobalA() != 0)
    {
        return 1;
    }

    if (testTypedefStructGlobalB() != 0)
    {
        return 1;
    }

    if (testTypedefStructPtr() != 0)
    {
        return 1;
    }

    if (testTypedefStructSelf() != 0)
    {
        return 1;
    }

    if (testStructsCircularRefLocal() != 0)
    {
        return 1;
    }

    if (testStructsCircularRefGlobal() != 0)
    {
        return 1;
    }

    if (testStructsCircularRefGlobalPtr() != 0)
    {
        return 1;
    }
    if (testStructAllign() != 0)
    {
        return 1;
    }
    return 0;
}
