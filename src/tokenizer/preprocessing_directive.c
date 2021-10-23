#include "../hooligan.h"

// indexに依存したコードを書いているので後方に追加していくこと
static char *preprocessing_directive_list[6] = {
    "include",
    "define",
    "ifdef",
    "ifndef",
    "endif",
    "line",
};

static int preprocessing_directive_list_count = 6;

static bool isdirective(char *p)
{
    for (int i = 0; i < preprocessing_directive_list_count; i++)
    {
        char *str = preprocessing_directive_list[i];
        if (strncmp(p, str, h_strlen(str)) == 0)
        {
            return true;
        }
    }
    return false;
}
