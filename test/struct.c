int testStructPtr()
{
    struct
    {
        int x;
        int y;
        int z;
    } * v;

    struct
    {
        int x;
        int y;
        int z;
    } w;
    v = &w;
    (*v).x = 3;
    (*v).y = 4;
    (*v).z = 5;

    if ((*v).x != 3)
    {
        return 1;
    }
    if ((*v).y != 4)
    {
        return 1;
    }
    if ((*v).z != 5)
    {
        return 1;
    }
    if (v->x != 3)
    {
        return 1;
    }
    if (v->y != 4)
    {
        return 1;
    }
    if (v->z != 5)
    {
        return 1;
    }
    return 0;
}

int testStruct()
{
    struct
    {
        int val;
        int offset;
    } Hello;
    Hello.val = 10;
    Hello.offset = 32;
    int x;
    x = Hello.val;
    if (Hello.val != 10)
    {
        return Hello.val;
    }
    if (x != 10)
    {
        return x;
    }

    if (Hello.offset != 32)
    {
        return 1;
    }
    return 0;
}

struct
{
    int x;
    int y;
    int z;
} vector;

int modv(int x, int y, int z)
{
    vector.x = x;
    vector.y = y;
    vector.z = z;
}

int testStructGvar()
{
    vector.x = 10;
    vector.y = 100;
    vector.z = 1000;
    if (vector.x != 10)
    {
        return 1;
    }
    if (vector.y != 100)
    {
        return 1;
    }
    if (vector.z != 1000)
    {
        return 1;
    }
    modv(6, 7, 8);
    if (vector.x != 6)
    {
        return 1;
    }
    if (vector.y != 7)
    {
        return 1;
    }
    if (vector.z != 8)
    {
        return 1;
    }
    return 0;
}

int testStructArray()
{
    struct
    {
        int val;
        int offset;
        int a[10];
    } b;
    b.val = 12;
    b.offset = 2;
    for (int i = 0; i < 10; i = i + 1)
    {
        b.a[i] = i + 1;
    }

    int res = 0;
    for (int i = 0; i < 10; i = i + 1)
    {
        res = res + b.a[i];
    }
    if (res != 55)
    {
        return 1;
    }
    return 0;
}

int testStructChar()
{
    struct
    {
        int val;
        int offset;
        char c;
    } d;
    d.val = 12;
    d.offset = 2;
    d.c = 8;
    d.c = d.c + d.val;
    if (d.c != 20)
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

    if (testStructPtr() != 0)
    {
        return 1;
    }

    if (testStructGvar() != 0)
    {
        return 1;
    }

    if (testStructArray() != 0)
    {
        return 1;
    }

    if (testStructChar() != 0)
    {
        return 1;
    }

    return 0;
}
