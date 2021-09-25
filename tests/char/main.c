#include <string.h>
int s;
int testCharGvar()
{
    char x[3];
    char y;
    char z;
    int a;
    int b;

    y = 5;
    z = -2;
    x[0] = -1;
    x[1] = 2;
    a = 11;
    b = 2;
    s = -3;
    if (x[0] != -1)
    {
        return 1;
    }
    if (y != 5)
    {
        return 1;
    }

    if (a + y != 16)
    {
        return 1;
    }

    if (z + b != 0)
    {
        return 1;
    }

    if (z - b != -4)
    {
        return 1;
    }

    if (s - x[0] != -2)
    {
        return 1;
    }
    return 0;
}

int testChar()
{
    char x[3];
    char y;
    char z;
    char w;
    int a;
    x[0] = -1;
    y = 1;
    z = 127;
    if (x[0] != -1)
    {
        return 1;
    }
    a = z + 1;
    if (z + 1 != 128)
    {
        return 1;
    }

    if (1 + z != 128)
    {
        return 1;
    }
    if (a != 128)
    {
        return 1;
    }
    w = z + y;
    if (w != -128)
    {
        return 1;
    }
    return 0;
}

int testStringEmpty()
{
    char *x;
    x = "";
    if (x[0] != 0)
    {
        return 1;
    }
    return 0;
}

int testString()
{
    char *y;
    char *z;
    char *v;

    y = "hellllo";
    v = "omg";
    z = y;

    if (y[0] != 104)
        return 1;
    if (y[1] != 101)
        return 1;
    if (y[2] != 108)
        return 1;
    if (y[3] != 108)
        return 1;
    if (y[4] != 108)
        return 1;
    if (y[5] != 108)
        return 1;
    if (y[6] != 111)
        return 1;
    if (z[0] != 104)
        return 1;
    if (z[1] != 101)
        return 1;
    if (z[2] != 108)
        return 1;
    if (z[3] != 108)
        return 1;
    if (z[4] != 108)
        return 1;
    if (z[5] != 108)
        return 1;
    if (z[6] != 111)
        return 1;

    return 0;
}

int testCharLiteral()
{
    char c = 'a';
    if (c != 97)
    {
        return 1;
    }
    c += 1;
    if (c != 'b')
    {
        return 1;
    }
    if ('z' - 'a' != 25)
    {
        return 1;
    }
    if ('Z' - 'A' != 25)
    {
        return 1;
    }
    if ('"' != 34)
    {
        return 1;
    }
    return 0;
}

int testCharEscape()
{
    char a = '\n';
    char b = '\0';
    char c = '\\';
    char d = '\"';
    char e = '"';

    if (a != 10 || a != '\n')
    {
        return 1;
    }

    if (b != 0 || b != '\0')
    {
        return 1;
    }

    if (c != 92 || c != '\\')
    {
        return 1;
    }

    if (d != 34 || d != '\"')
    {
        return 1;
    }

    if (e != 34 || e != '\"')
    {
        return 1;
    }
    if (e != 34 || e != '"')
    {
        return 1;
    }

    return 0;
}

int testStringWithEscape()
{
    char *x;
    char *y;
    char *z;
    char *v;
    char *w;
    x = "abc\nd";
    y = "abc\"d";
    z = "abc\"\"d";
    v = "abc\"\"\\d";
    w = "abc\\\\d";
    if (strlen(x) != 5)
    {
        return 1;
    }
    if (strlen(y) != 5)
    {
        return 1;
    }
    if (strlen(z) != 6)
    {
        return 1;
    }
    if (strlen(v) != 7)
    {
        return 1;
    }

    if (strlen(w) != 6)
    {
        return 1;
    }
    return 0;
}

int main()
{
    if (testChar() != 0)
    {
        return 1;
    }
    if (testCharGvar() != 0)
    {
        return 1;
    }
    if (testStringEmpty() != 0)
    {
        return 1;
    }
    if (testString() != 0)
    {
        return 1;
    }

    if (testCharLiteral() != 0)
    {
        return 1;
    }

    if (testCharEscape() != 0)
    {
        return 1;
    }
    if (testStringWithEscape() != 0)
    {
        return 1;
    }

    return 0;
}
