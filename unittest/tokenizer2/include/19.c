#ifndef HOGE
#define HOGE
int hogege()
{
    return 10;
}
#endif
#ifndef HOGE
int mmmm()
{
    return 0;
}
#endif
#ifdef HOGE
int main()
{
    return 0;
}
#endif