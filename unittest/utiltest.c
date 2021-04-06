#include "../util.c"

int main()
{
    if (!(true))
        return 1;
    char *s1 = "hogehoge";
    char *s2 = "fugafuga";
    char *s;
    s = join_str(s1, s2);
    printf("%s\n", s);
    s = insert_str(s1, 0, s2);
    printf("%s\n", s);
    s = insert_str(s1, 1, s2);
    printf("%s\n", s);
    s = insert_str(s1, 2, s2);
    printf("%s\n", s);
    s = insert_str(s1, strlen(s1), s2);
    printf("%s\n", s);
    return 0;
}
