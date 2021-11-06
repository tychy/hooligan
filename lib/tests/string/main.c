#include <stdio.h>
#include "../../string.c"

int main()
{
    if (h_strlen("") != 0)
    {
        printf("The length of \"\" is expected to 1\n");
        return 1;
    }

    if (h_strlen("hogehoge") != 8)
    {
        printf("The length of \"hogehoge\" is expectexd to 8\n");
        return 1;
    }

    char *c = "hogehoge\nfugafuga\npiyopiyo";
    if (h_strlen(c) != 26)
    {
        printf("The length of \"%s\" is expectexd to 26", c);
        return 1;
    }
    return 0;
}
