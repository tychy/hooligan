#include <stdlib.h>
int testIntCharCast()
{
    int a = 10;
    char b = (char)a;
    if (b != 10)
    {
        printf("b%d\n", b);
        return 1;
    }
    return 0;
}

int testDynamicArray()
{
    int *arr;
    arr = (int *)malloc(10 * sizeof(int));
    arr[0] = 1;
    arr[9] = 11;
    if (arr[0] != 1)
    {
        return 1;
    }
    if (arr[9] != 11)
    {
        return 1;
    }
    int *dummy = (int *)arr;

    if (dummy[0] != 1)
    {
        return 1;
    }
    arr[9] = 111;
    if (dummy[9] != 111)
    {
        return 1;
    }
    return 0;
}
int main()
{
    if (testIntCharCast() != 0)
    {
        return 1;
    }

    if (testDynamicArray() != 0)
    {
        return 1;
    }
    return 0;
}