#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void p(char *buf)
{
}

char *prints1(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char *raw = calloc(100, sizeof(char));
    vsprintf(raw, fmt, ap);
    p(raw);
    va_end(ap);
    return raw;
}

char *prints2(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char *raw = calloc(100, sizeof(char));
    vsprintf(raw, fmt, ap);
    int a = 3;
    p(raw);
    va_end(ap);
    return raw;
}

char *inter()
{
    char *s = prints1("piyopiyo");
    return s;
}

char *inter2()
{
    char *s = inter();
    return s;
}

char *inter3()
{
    char *s = inter2();
    return s;
}

int main()
{
    char *s = prints1("hogehoge");
    char *s1 = prints2("fugafuga");
    char *s2 = inter();
    char *s3 = inter2();
    char *s4 = inter3();
    printf("%s\n", s);
    printf("%s\n", s1);
    printf("%s\n", s2);
    printf("%s\n", s3);
    printf("%s\n", s4);

    return 0;
}
