#include "../util.c"

int main()
{
    if (not(true))
        return 1;
    char *s1 = "hogehoge";
    char *s2 = "fugafuga";
    char *s3 = join_str(s1, s2);
    printf("%s\n", s3);
    return 0;
}
