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

struct PPToken
{
    PPTokenKind kind;
    PPToken *next;
    int val;
    int len;
    char *str;
};

// note: 文字数の多いものを先に登録する
// note: 要素数を更新する
static char *punctuator_list[33] = {
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
};

static int punctuator_list_count = 33;

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

        // ブロックコメントをスキップ
        if (strncmp(p, "#", 1) == 0)
        {
            p += 1;
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

        if (*p == 39) // 39 = '
        {             // ' バックスラッシュによるエスケープを使いたくなかった
            p++;
            if (*p == 92)
            {
                p++;
                if (*p == 92)
                {
                    cur = new_token(PPTK_CHAR, cur, p);
                    cur->val = '\\';
                }
                else if (*p == '0')
                {
                    cur = new_token(PPTK_CHAR, cur, p);
                    cur->val = '\0';
                }
                else if (*p == 'n')
                {
                    cur = new_token(PPTK_CHAR, cur, p);
                    cur->val = '\n';
                }
                else if (*p == 34)
                {
                    cur = new_token(PPTK_CHAR, cur, p);
                    cur->val = '\"';
                }
                else
                {
                    error("未定義のエスケープ文字です");
                }
            }
            else
            {
                cur = new_token(PPTK_CHAR, cur, p);
                cur->val = *p;
            }
            cur->len = 1;
            p++;
            if (*p != 39)
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
            printf("HN ");
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
