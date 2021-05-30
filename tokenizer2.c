#include "hooligan.h"

typedef enum
{
    PPTK_HN, // header-name
    PPTK_IDENT,
    PPTK_NUMBER,
    PPTK_CHAR, // character-constant
    PPTK_STRING,
    PPTK_PUNC, // punctuators
} PPTokenKind;

typedef struct PPToken PPToken;
typedef struct Macro Macro;
typedef struct PPContext PPContext;

struct PPToken
{
    PPTokenKind kind;
    PPToken *next;
    int val;
    int len;
    char *str;
};

struct Macro
{
    PPToken *target;
    PPToken *replace;
    Macro *next;
};
struct PPContext
{
    Macro *macros;
};

extern PPContext *pp_ctx;

// note: 文字数の多いものを先に登録する
// note: 要素数を更新する
static char *punctuator_list[34] = {
    "++",
    "--",
    "+=",
    "-=",
    "*=",
    "%=",
    "==",
    "!=",
    ">=",
    "<=",
    "->",
    "&&",
    "||",
    ">",
    "<",
    "+",
    "-",
    "*",
    "/",
    "%",
    "(",
    ")",
    "=",
    ";",
    "{",
    "}",
    ",",
    "&",
    "[",
    "]",
    ".",
    "!",
    ":",
    "#",
};

static int punctuator_list_count = 34;

static char *preprocessing_directive_list[2] = {
    "include",
    "define",
};

static int preprocessing_directive_list_count = 2;

static PPToken *new_token(PPTokenKind kind, PPToken *cur, char *str)
{
    PPToken *tok = calloc(1, sizeof(PPToken));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
}

// nondigit = [_a-zA-Z]
// 識別子を構成する文字で数字でないもの
static bool isnondigit(char p)
{
    if ((p >= 'a' && p <= 'z') || (p >= 'A' && p <= 'Z') || p == '_')
    {
        return true;
    }
    return false;
}

// punctuator...区切り文字
static bool ispunctuator(char *p)
{
    for (int i = 0; i < punctuator_list_count; i++)
    {
        char *str = punctuator_list[i];
        if (strncmp(p, str, strlen(str)) == 0)
        {
            return true;
        }
    }
    return false;
}

static bool isdirective(char *p)
{
    for (int i = 0; i < preprocessing_directive_list_count; i++)
    {
        char *str = preprocessing_directive_list[i];
        if (strncmp(p, str, strlen(str)) == 0)
        {
            return true;
        }
    }
    return false;
}

static int from_escape_char_to_int(char p)
{
    if (p == '\\')
    {
        return '\\';
    }
    else if (p == '0')
    {
        return '\0';
    }
    else if (p == 'n')
    {
        return '\n';
    }
    else if (p == '\'')
    {
        return '\'';
    }
    else
    {
        printf("%c ", p);
        error("未定義のエスケープ文字です");
    }
}

PPToken *decompose_to_pp_token(char *p)
{
    PPToken head;
    head.next = NULL;
    PPToken *cur = &head;

    while (*p)
    {
        if (isspace(*p))
        {
            p++;
            continue;
        }

        if (strncmp(p, "//", 2) == 0)
        {
            p += 2;
            while (*p != '\n')
                p++;
            continue;
        }

        if (strncmp(p, "/*", 2) == 0)
        {
            char *q = strstr(p + 2, "*/");
            if (!q)
                error("コメントが閉じられていません");
            p = q + 2;
            continue;
        }

        // プリプロセッシング命令文の処理、暴力的な長さなので切り出しを検討すべき
        if (strncmp(p, "#", 1) == 0)
        {
            cur = new_token(PPTK_PUNC, cur, p);
            cur->len = 1;
            p++;
            if (!isdirective(p))
                error("未定義のプリプロセッシング命令文です");
            int directive_index;
            for (int i = 0; i < preprocessing_directive_list_count; i++)
            {
                char *directive = preprocessing_directive_list[i];
                if (strncmp(p, directive, strlen(directive)) == 0)
                {
                    cur = new_token(PPTK_IDENT, cur, p);
                    cur->len += strlen(directive);
                    p += strlen(directive);
                    directive_index = i;
                }
            }
            if (directive_index == 0)
            {
                // include文
                while (isspace(*p))
                {
                    p++;
                }
                if (*p == '<')
                {
                    p++;
                    int len = 1;
                    while (*p != '\n')
                    {
                        if (*p == '>')
                        {
                            cur = new_token(PPTK_HN, cur, p - len);
                            cur->len = len + 1;
                            break;
                        }
                        len++;
                        p++;
                    }
                    if (cur->kind != PPTK_HN)
                    {
                        error("不正なinclude文です");
                    }
                }
                else if (*p == '"')
                {
                    p++;
                    int len = 1;
                    while (*p != '\n')
                    {
                        if (*p == '"')
                        {
                            cur = new_token(PPTK_HN, cur, p - len);
                            cur->len = len + 1;
                            break;
                        }
                        len++;
                        p++;
                    }
                    if (cur->kind != PPTK_HN)
                    {
                        error("不正なinclude文です");
                    }
                }
                else
                {
                    error("不正なinclude文です");
                }
            }
            else if (directive_index == 1)
            {
                // define文
                // #define ident identを想定

                while (isspace(*p))
                {
                    p++;
                }

                int i = 0;
                char *p_top = p;
                while (isnondigit(*p) || isdigit(*p))
                {
                    i++;
                    p++;
                }
                cur = new_token(PPTK_IDENT, cur, p_top);
                cur->len = i;

                while (isspace(*p))
                {
                    p++;
                }
                i = 0;
                p_top = p;
                while (isnondigit(*p) || isdigit(*p))
                {
                    i++;
                    p++;
                }
                cur = new_token(PPTK_IDENT, cur, p_top);
                cur->len = i;
            }
            else
            {
                error("未定義のプリプロセッシング命令文です");
            }
            while (*p != '\n')
                p++;
            continue;
        }

        if (*p == '"')
        {
            int i = 0;
            p++;
            char *p_top = p;
            for (;;)
            {
                if (*p == '\\')
                {
                    p++;
                    i++;
                    if (*p == '"')
                    {
                        p++;
                        i++;
                    }
                }
                else if (*p == '"')
                {
                    break;
                }
                else
                {
                    p++;
                    i++;
                }
            }
            if (*p != '"')
                error("ダブルクォテーションが閉じていません");

            p++;
            cur = new_token(PPTK_STRING, cur, p_top);
            cur->len = i;

            continue;
        }

        if (*p == '\'')
        {
            p++;
            int val;
            if (*p == '\\')
            {
                p++;
                val = from_escape_char_to_int(*p);
            }
            else
            {
                val = *p;
            }
            cur = new_token(PPTK_CHAR, cur, p);
            cur->val = val;
            p++;
            if (*p != '\'')
            {
                error("シングルクォーテーションが閉じていません");
            }
            p++;
            continue;
        }

        if (isnondigit(*p))
        {
            int i = 0;
            char *p_top = p;
            while (isnondigit(*p) || isdigit(*p))
            {
                i++;
                p++;
            }
            cur = new_token(PPTK_IDENT, cur, p_top);
            cur->len = i;
            continue;
        }

        if (ispunctuator(p))
        {
            for (int i = 0; i < punctuator_list_count; i++)
            {

                char *op = punctuator_list[i];
                if (strncmp(p, op, strlen(op)) == 0)
                {
                    cur = new_token(PPTK_PUNC, cur, op);
                    cur->len = strlen(op);
                    p += strlen(op);
                }
            }
            continue;
        }

        if (isdigit(*p))
        {
            cur = new_token(PPTK_NUMBER, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }
        printf("%sトークナイズできません", p);
        exit(1);
    }
    return head.next;
}

Macro *find_macro(char *str, int len)
{
    for (Macro *mac = pp_ctx->macros; mac; mac = mac->next)
    {
        if (mac->target->len == len && strncmp(mac->target->str, str, len) == 0)
        {
            return mac;
        }
    }
    return NULL;
}

PPToken *preprocess_directives(char *base_dir, PPToken *tok)
{
    PPToken *prev = tok;
    PPToken *cur = tok;

    while (cur)
    {
        // マクロの登録
        if (cur->kind == PPTK_PUNC && *cur->str == '#')
        {
            if (cur->next->kind == PPTK_IDENT &&
                strncmp(cur->next->str, preprocessing_directive_list[1], cur->next->len) == 0)
            {
                PPToken *target = cur->next->next;
                PPToken *replace = cur->next->next->next;

                if (target->kind == PPTK_IDENT && replace->kind == PPTK_IDENT)
                {
                    if (target->len == replace->len && strncmp(target->str, replace->str, target->len) == 0)
                    {
                        error("target == replace　のマクロは定義できません");
                    }
                    if (find_macro(target->str, target->len) != NULL)
                    {
                        error("マクロの二重定義です");
                    }
                    Macro *mac = calloc(1, sizeof(Macro));
                    mac->target = target;
                    mac->replace = replace;
                    mac->next = pp_ctx->macros;
                    pp_ctx->macros = mac;
                }
                else
                {
                    error("#define ident ident　である必要があります");
                }
                if (prev == cur)
                {
                    prev = replace->next;
                    cur = replace->next;
                    tok = prev;
                }
                else
                {
                    prev->next = replace->next;
                    cur = replace->next;
                }
                continue;
            }
        }
        // マクロの検索
        if (cur->kind == PPTK_IDENT)
        {
            for (;;)
            {
                Macro *mac = find_macro(cur->str, cur->len);
                if (mac)
                {
                    cur->str = mac->replace->str;
                    cur->len = mac->replace->len;
                }
                else
                {
                    break;
                }
            }
        }

        prev = cur;
        cur = cur->next;
    }
    return tok;
}

void dump_pp_token(PPToken *tok)
{
    PPToken *cur = tok;
    while (cur)
    {
        switch (cur->kind)
        {
        case PPTK_CHAR:
            printf("%d ", cur->val);
            break;
        case PPTK_HN:
            printf("%.*s ", cur->len, cur->str);
            break;
        case PPTK_IDENT:
            printf("%.*s ", cur->len, cur->str);
            break;
        case PPTK_NUMBER:
            printf("%d ", cur->val);
            break;
        case PPTK_PUNC:
            printf("%.*s ", cur->len, cur->str);
            break;
        case PPTK_STRING:
            printf("\\\"%.*s\\\" ", cur->len, cur->str);
            break;
        }
        cur = cur->next;
    }
    printf("\n");
}
