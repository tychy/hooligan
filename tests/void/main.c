int main()
{
    int a = 3;
    void *hoge;
    hoge = &a;
    int *b = (int *)hoge;
    if (*b != 3){
        return 1;
    }
    char *c;
    c = (char *)hoge;
    if (*c != 3){
        return 1;
    }
    return 0;
}