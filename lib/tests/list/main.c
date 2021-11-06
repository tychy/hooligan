#include <stdlib.h>
#include "../../list.c"

static int assert_int(int expected, int actual)
{
    if (expected == actual)
    {
        return 0;
    }
    return 1;
}

int testAppend()
{
    List *a = new_list(5);
    int elms[6] = {92, 789, 628, 95, 721, 116};
    for (int i = 0; i < 6; i++)
    {
        a = append(a, &elms[i]);
    }
    int last = 225;
    a = append(a, &last);

    if (assert_int(92, *((int *)a->elm[0])))
    {
        return 1;
    }

    if (assert_int(789, *((int *)a->elm[1])))
    {
        return 1;
    }

    if (assert_int(628, *((int *)a->elm[2])))
    {
        return 1;
    }

    if (assert_int(95, *((int *)a->elm[3])))
    {
        return 1;
    }

    if (assert_int(721, *((int *)a->elm[4])))
    {
        return 1;
    }

    if (assert_int(116, *((int *)a->elm[5])))
    {
        return 1;
    }
    return 0;
}

int listcall(List *a)
{
    if (assert_int(92, *((int *)a->elm[0])))
    {
        return 1;
    }

    if (assert_int(789, *((int *)a->elm[1])))
    {
        return 1;
    }

    if (assert_int(628, *((int *)a->elm[2])))
    {
        return 1;
    }

    if (assert_int(95, *((int *)a->elm[3])))
    {
        return 1;
    }

    if (assert_int(721, *((int *)a->elm[4])))
    {
        return 1;
    }

    if (assert_int(116, *((int *)a->elm[5])))
    {
        return 1;
    }
    return 0;
}

int testHeapAppend()
{
    List *a = new_list(2);
    int elms[6] = {92, 789, 628, 95, 721, 116};
    for (int i = 0; i < 6; i++)
    {
        int *tmp = calloc(1, sizeof(int));
        *tmp = elms[i];
        a = append(a, tmp);
    }
    int last = 225;
    a = append(a, &last);
    if (assert_int(92, *((int *)a->elm[0])))
    {
        return 1;
    }

    if (assert_int(789, *((int *)a->elm[1])))
    {
        return 1;
    }

    if (assert_int(628, *((int *)a->elm[2])))
    {
        return 1;
    }

    if (assert_int(95, *((int *)a->elm[3])))
    {
        return 1;
    }

    if (assert_int(721, *((int *)a->elm[4])))
    {
        return 1;
    }

    if (assert_int(116, *((int *)a->elm[5])))
    {
        return 1;
    }

    if (listcall(a) != 0)
    {
        return 1;
    }
    return 0;
}

int testNullAppend()
{
    List *a = NULL;
    int x = 10;
    a = append(a, &x);
    if (assert_int(10, *((int *)a->elm[0])))
    {
        return 1;
    }
    return 0;
}

int main()
{
    if (testAppend() != 0)
    {
        return 1;
    }

    if (testHeapAppend() != 0)
    {
        return 1;
    }
    if (testNullAppend() != 0)
    {
        return 1;
    }
    return 0;
}
