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

int main()
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
