#include <stdarg.h>

int testFunc()
{
    int x = 10;
    if (11 - x != 1)
    {
        return 1;
    }
    return 0;
}
int sums(int x, int y)
{
    return x + y;
}
int testFuncTooMany()
{
    int x = 10;
    int y = 22;
    if (sums(x, y) != 32)
    {
        return 1;
    }
    return 0;
}
int a;
int g[10];

int foo(int *arr)
{
    return arr[2];
}

int *bar(int *b)
{
    return b;
}

int testFuncPtr()
{
    int *p;
    a = 11;
    p = bar(&a);
    if (*p != 11)
    {
        return 1;
    }
    if (*(bar(&a)) != 11)
    {
        return 1;
    }
    g[0] = 1;
    g[1] = 2;
    g[2] = 3;
    if (foo(g) != 3)
    {
        return 1;
    }
    return 0;
}
int foovoid(void)
{
    return 11;
}
int testFuncArgVoid()
{
    if (foovoid() != 11)
    {
        return 1;
    }
    return 0;
}
int z;
void setz()
{
    z = 20;
    return;
}
void funcHasCharArgument(char p)
{
    return;
}
int testFuncVoid()
{
    setz();
    if (z != 20)
    {
        return 1;
    }
    return 0;
}
typedef struct Hoge Hoge;
struct Hoge
{
    int index;
    int hash;
    int val;
};

int func_hoge_val(Hoge *h[10])
{
    return h[0]->val + h[1]->val + h[2]->val;
}

int func_hoge_index(Hoge *h[10])
{
    return h[0]->index + h[1]->index + h[2]->index;
}

int func_array(int c[10])
{
    return c[0] + c[1];
}

int testFuncArray()
{
    int a[10];
    a[0] = 1;
    a[1] = 2;

    if (func_array(a) != 3)
    {
        return 1;
    }
    return 0;
}

int testFuncStructArrayLocal()
{
    Hoge *hogearray[10];
    Hoge ha;
    Hoge hb;
    Hoge hc;
    ha.index = 100;
    hb.index = 200;
    hc.index = 300;

    ha.val = 1;
    hb.val = 2;
    hc.val = 3;

    hogearray[0] = &ha;
    hogearray[1] = &hb;
    hogearray[2] = &hc;
    if (func_hoge_val(hogearray) != 6)
    {
        return 1;
    }

    if (func_hoge_index(hogearray) != 600)
    {
        return 1;
    }
    return 0;
}
Hoge *hogeglob[10];
Hoge haglob;
int func_hoge_glob(Hoge *h[10])
{
    return h[0]->val + h[1]->val + h[2]->val;
}

int testFuncStructArrayGlobal()
{

    Hoge *hbglob = calloc(1, sizeof(Hoge));
    Hoge *hcglob = calloc(1, sizeof(Hoge));
    haglob.val = 3;
    hbglob->val = 22;
    hcglob->val = 33;
    hogeglob[0] = &haglob;
    hogeglob[1] = hbglob;
    hogeglob[2] = hcglob;

    if (func_hoge_glob(hogeglob) != 58)
    {
        return 1;
    }
    return 0;
}
int testFuncHasCharArgument()
{
    funcHasCharArgument('a');
    return 0;
}

int sum(int count, ...)
{
    va_list ap;
    va_start(ap, count);
    int res = 0;
    for (int i = 0; i < count; i++)
    {
        res += va_arg(ap, int);
    }
    va_end(ap);
    return res;
}

int testFuncHasVariableLengthArguments()
{
    if (sum(1, 13) != 13)
    {
        return 1;
    }
    if (sum(2, 3, 4) != 7)
    {
        return 1;
    }
    if (sum(3, 134, 765, 35) != 934)
    {
        return 1;
    }
    if (sum(4, 33, 44, 55, 66) != 198)
    {
        return 1;
    }
    return 0;
}

int pass_by_val(int a)
{
    a += 100;
    return a;
}

int pass_by_ref(int *a)
{
    *a += 100;
    return a;
}

int testCallByReference()
{
    int a = 100;
    pass_by_val(a);
    if (a != 100)
    {
        return 1;
    }
    pass_by_ref(&a);
    if (a != 200)
    {
        return 1;
    }
}

int main()
{
    if (testFunc() != 0)
    {
        return 1;
    }
    if (testFuncTooMany() != 0)
    {
        return 1;
    }

    if (testFuncPtr() != 0)
    {
        return 1;
    }
    if (testFuncArgVoid() != 0)
    {
        return 1;
    }
    if (testFuncArray() != 0)
    {
        return 1;
    }

    if (testFuncStructArrayLocal() != 0)
    {
        return 1;
    }

    if (testFuncStructArrayGlobal() != 0)
    {
        return 1;
    }
    if (testFuncHasCharArgument() != 0)
    {
        return 1;
    }
    if (testFuncHasVariableLengthArguments() != 0)
    {
        return 1;
    }

    return 0;
}
