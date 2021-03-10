int main()
{
    struct
    {
        int x;
        int y;
        int z;
    } *vector;


    (*vector).x = 3;
    (*vector).y = 4;
    (*vector).z = 5;

    if ((*vector).x != 3)
    {
        return 1;
    }
    if ((*vector).y != 4)
    {
        return 1;
    }
    if ((*vector).z != 5)
    {
        return 1;
    }
    return 0;
}
