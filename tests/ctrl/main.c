int testCtrl()
{
    int a;

    a = 0;
    for (int counter = 0; counter < 100; counter = counter + 1)
        if (counter * counter == 400)
            a = counter;
    if (a != 20)
        return 1;

    a = 0;
    int i;
    i = 0;
    while (i < 10)
        if (i == 5)
        {
            a = i;
            i += 1;
        }
        else
            i += 1;
    if (a != 5)
        return 1;
    return 0;
}

int testBreak()
{
    int res = -1;
    for (int i = 0; i < 100; i += 1)
    {
        if (i == 50)
        {
            res = i;
            break;
        }
    }
    if (res != 50)
    {
        return 1;
    }
    return 0;
}

int testForContinue()
{
    int i;
    int x;
    for (i = 0; i < 10; i++)
    {
        x = i;
        continue;
    }
    if (i != 10)
    {
        return 1;
    }
    return 0;
}
int testWhileContinue()
{
    int total = 0;
    int i = 0;
    while (i <= 10)
    {
        if (i / 2 != i - i / 2) // 奇数
        {
            i += 1;
            continue;
        }
        total = total + i;
        i += 1;
    }
    if (total != 30)
    {
        return 1;
    }
    return 0;
}

int testPostIncrement()
{
    int sum_one = 0;
    int sum_two = 0;
    int cnt = 0;
    for (int i = 0; i < 100; i++)
    {
        // 両者同じ値が足されるはず
        sum_one += i;
        sum_two += cnt++;
    }
    if (cnt != 100)
    {
        return 1;
    }
    if (sum_one != 4950)
    {
        return 1;
    }
    if (sum_two != 4950)
    {
        return 1;
    }
    return 0;
}
int testPostIncrementIntPtr()
{

    int x[4] = {1, 2, 3, 4};
    int *y = x;
    if (*y != 1)
    {
        return 1;
    }
    if (*y++ != 1)
    {
        return 1;
    }
    if (*y != 2)
    {
        return 1;
    }
    y++;
    if (*y != 3)
    {
        return 1;
    }
    return 0;
}
int testPostIncrementCharPtr()
{
    char *x = "abcde";
    if (*x != 'a')
    {
        return 1;
    }
    x++;
    if (*x != 'b')
    {
        return 1;
    }
    if (*x++ != 'b')
    {
        return 1;
    }
    if (*x != 'c')
    {
        return 1;
    }
    if (*x != 'c')
    {
        return 1;
    }
    return 0;
}

int testPostIncrementStruct()
{
    typedef struct Hoge Hoge;
    struct Hoge
    {
        int val;
    };
    Hoge x;
    Hoge *y;
    y = calloc(1, sizeof(Hoge));
    y->val = 100;
    x.val = 10;
    if (x.val != 10)
    {
        return 1;
    }

    if (x.val++ != 10)
    {
        return 1;
    }
    if (x.val != 11)
    {
        return 1;
    }

    if (y->val != 100)
    {
        return 1;
    }

    if (y->val++ != 100)
    {
        return 1;
    }
    if (y->val != 101)
    {
        return 1;
    }
    return 0;
}

int testPostDecrement()
{
    int sum_one = 0;
    int sum_two = 0;
    int cnt = 99;
    for (int i = 0; i < 100; i++)
    {
        // 両者同じ値が足されるはず
        sum_one += i;
        sum_two += cnt--;
    }
    if (cnt != -1)
    {
        return 1;
    }
    if (sum_one != 4950)
    {
        return 1;
    }
    if (sum_two != 4950)
    {
        return 1;
    }
    return 0;
}
int testPostDecrementIntPtr()
{

    int x[4] = {1, 2, 3, 4};
    int *y = x + 3;
    if (*y != 4)
    {
        return 1;
    }
    if (*y-- != 4)
    {
        return 1;
    }
    if (*y != 3)
    {
        return 1;
    }
    y--;
    if (*y != 2)
    {
        return 1;
    }
    return 0;
}
int testPostDecrementCharPtr()
{
    char *y = "abcde";
    char *x = y + 4;
    if (*x != 'e')
    {
        return 1;
    }
    x--;
    if (*x != 'd')
    {
        return 1;
    }
    if (*x-- != 'd')
    {
        return 1;
    }
    if (*x != 'c')
    {
        return 1;
    }
    return 0;
}

int testPostDecrementStruct()
{
    typedef struct Hoge Hoge;
    struct Hoge
    {
        int val;
    };
    Hoge x;
    Hoge *y;
    y = calloc(1, sizeof(Hoge));
    y->val = 100;
    x.val = 10;
    if (x.val != 10)
    {
        return 1;
    }

    if (x.val-- != 10)
    {
        return 1;
    }
    if (x.val != 9)
    {
        return 1;
    }

    if (y->val != 100)
    {
        return 1;
    }

    if (y->val-- != 100)
    {
        return 1;
    }
    if (y->val != 99)
    {
        return 1;
    }
    return 0;
}
int main()
{
    if (testCtrl() != 0)
    {
        return 1;
    }
    if (testBreak() != 0)
    {
        return 1;
    }
    if (testForContinue() != 0)
    {
        return 1;
    }
    if (testWhileContinue() != 0)
    {
        return 1;
    }
    if (testPostIncrement() != 0)
    {
        return 1;
    }
    if (testPostIncrementIntPtr() != 0)
    {
        return 1;
    }
    if (testPostIncrementCharPtr() != 0)
    {
        return 1;
    }
    if (testPostDecrement() != 0)
    {
        return 1;
    }

    if (testPostDecrementIntPtr() != 0)
    {
        return 1;
    }

    if (testPostDecrementCharPtr() != 0)
    {
        return 1;
    }
    return 0;
}
