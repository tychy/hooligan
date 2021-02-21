#include <stdio.h>

int foo() { return 10; }

int hello_world()
{
    printf("Hello World!\n");
    return 1;
}

int two(int x)
{
    return 2 * x;
}

int BIG(int x)
{
    return x + 10;
}

int add(int x, int y)
{
    return x + y;
}

int add_tri(int x, int y, int z)
{
    return x + y + z;
}

int add_qua(int x, int y, int z, int a)
{
    return x + y + z + a;
}

int add_pen(int x, int y, int z, int a, int b)
{
    return x + y + z + a + b;
}
