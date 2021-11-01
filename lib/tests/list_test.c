#include "../list.c"

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
    assert_int(92, *((int *)a->elm[0]));
    assert_int(789, *((int *)a->elm[1]));
    assert_int(628, *((int *)a->elm[2]));
    assert_int(95, *((int *)a->elm[3]));
    assert_int(721, *((int *)a->elm[4]));
    assert_int(116, *((int *)a->elm[5]));
    assert_int(last, *((int *)a->elm[6]));
    return 0;
}

int main()
{
    testAppend();
    return 0;
}
