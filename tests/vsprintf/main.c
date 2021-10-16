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

int main()
{
    char *s = prints1("hogehoge");
    char *s1 = prints2("fugafuga");
    printf("%s\n", s);
    printf("%s\n", s1);
    return 0;
}
