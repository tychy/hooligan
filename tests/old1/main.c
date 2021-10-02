int test1() { 5 + 21 - 4; }
int test2() { 50 + 121 - 40; }
int test3() { 50 + 121 - 40; }
int test4() { 12 + 34 - 5; }
int test5() { 2 * 5; }
int test6() { 7 * 2 * 3; }
int test7() { 1 + 3 * 4; }
int test8() { 5 - 2 * 3 + 7; }
int test9() { 4 * 5 * 2 + 6 * 7 - 5 * 9; }
int test10() { 5 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1; }
int test11() { 8 / 4; }
int test12() { 60 / 11; }
int test13() { 18 / 6 + 1 * 3; }
int test14() { 5 + 6 * 7; }
int test15() { 5 * (9 - 6); }
int test16() { (3 + 5) / 2; }
int test17() { (4 * 9) - (6 * 5) / 7; }
int test18() { 1 + -3 + 2; }
int test19() { -(-1); }
int test20() { 3 + -3; }
int test21() { -5 * -4; }
int test22() { -10 / -2; }
int test23() { 1 == 1; }
int test24() { 1 == 2; }
int test25() { 4 * 6 == 3 * 8; }
int test26() { 500 * 100 == 250 * 200; }
int test27() { (1 == 1) + 6; }
int test28() { 0 != 1; }
int test29() { 5 != 2 + 3; }
int test30() { 5 >= 2 + 3; }
int test31() { 5 > 2 + 3; }
int test32() { 5 <= 2 + 3; }
int test33() { 5 < 2 + 3; }
int test34() { 2 > 1; }
int test35() { 2 < 1; }
int test36() { 2 >= 1; }
int test37() { 2 <= 1; }
int test38() { (4 > 1) >= (1 < 0); }
int test39() { (4 > 1) > (1 < 0); }
int test40() { (4 > 1) <= (1 < 0); }
int test41() { (4 > 1) < (1 < 0); }
int test42()
{
    int a;
    a = 0;
}
int test43()
{
    int a;
    a = 3;
}
int test44()
{
    int a;
    int b;
    a = b = 3;
}
int test45()
{
    1 + 2;
    2 + 3;
}
int test46()
{
    1;
    1;
    1;
    1;
    1;
    1;
    1;
    1;
    1;
    3 + 3;
}
int test47()
{
    int a;
    a = 2;
    a;
}
int test48()
{
    int b;
    int c;
    b = 2;
    c = 3;
    b + c;
}
int test49()
{
    int foo;
    int bar;
    foo = 2;
    bar = 3;
    foo + bar;
}
int test50()
{
    int foo;
    int bar;
    int hogefuga;
    foo = 2;
    bar = 3;
    hogefuga = 9;
    hogefuga *bar;
}

int main()
{
    if (test1() != 22)
    {
        return 1;
    }
    if (test2() != 131)
    {
        return 1;
    }
    if (test3() != 131)
    {
        return 1;
    }
    if (test4() != 41)
    {
        return 1;
    }
    if (test5() != 10)
    {
        return 1;
    }
    if (test6() != 42)
    {
        return 1;
    }
    if (test7() != 13)
    {
        return 1;
    }
    if (test8() != 6)
    {
        return 1;
    }
    if (test9() != 37)
    {
        return 1;
    }
    if (test10() != 5)
    {
        return 1;
    }
    if (test11() != 2)
    {
        return 1;
    }
    if (test12() != 5)
    {
        return 1;
    }
    if (test13() != 6)
    {
        return 1;
    }
    if (test14() != 47)
    {
        return 1;
    }
    if (test15() != 15)
    {
        return 1;
    }
    if (test16() != 4)
    {
        return 1;
    }
    if (test17() != 32)
    {
        return 1;
    }
    if (test18() != 0)
    {
        return 1;
    }
    if (test19() != 1)
    {
        return 1;
    }
    if (test20() != 0)
    {
        return 1;
    }
    if (test21() != 20)
    {
        return 1;
    }
    if (test22() != 5)
    {
        return 1;
    }
    if (test23() != 1)
    {
        return 1;
    }
    if (test24() != 0)
    {
        return 1;
    }
    if (test25() != 1)
    {
        return 1;
    }
    if (test26() != 1)
    {
        return 1;
    }
    if (test27() != 7)
    {
        return 1;
    }
    if (test28() != 1)
    {
        return 1;
    }
    if (test29() != 0)
    {
        return 1;
    }
    if (test30() != 1)
    {
        return 1;
    }
    if (test31() != 0)
    {
        return 1;
    }
    if (test32() != 1)
    {
        return 1;
    }
    if (test33() != 0)
    {
        return 1;
    }
    if (test34() != 1)
    {
        return 1;
    }
    if (test35() != 0)
    {
        return 1;
    }
    if (test36() != 1)
    {
        return 1;
    }
    if (test37() != 0)
    {
        return 1;
    }
    if (test38() != 1)
    {
        return 1;
    }
    if (test39() != 1)
    {
        return 1;
    }
    if (test40() != 0)
    {
        return 1;
    }
    if (test41() != 0)
    {
        return 1;
    }
    if (test42() != 0)
    {
        return 1;
    }
    if (test43() != 3)
    {
        return 1;
    }
    if (test44() != 3)
    {
        return 1;
    }
    if (test45() != 5)
    {
        return 1;
    }
    if (test46() != 6)
    {
        return 1;
    }
    if (test47() != 2)
    {
        return 1;
    }
    if (test48() != 5)
    {
        return 1;
    }
    if (test49() != 5)
    {
        return 1;
    }
    if (test50() != 27)
    {
        return 1;
    }

    return 0;
}
