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

PPContext *pp_ctx;

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

// indexに依存したコードを書いているので後方に追加していくこと
static char *preprocessing_directive_list[5] = {
    "include",
    "define",
    "ifdef",
    "ifndef",
    "endif",
};

static int preprocessing_directive_list_count = 5;

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

static bool isdirective_idx(PPToken *tok, int idx)
{
    if (tok == NULL)
    {
        return false;
    }
    return (tok->kind == PPTK_IDENT &&
            strncmp(tok->str, preprocessing_directive_list[idx], strlen(preprocessing_directive_list[idx])) == 0);
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
            {
                error("未定義のプリプロセッシング命令文です");
            }
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
            else if (directive_index == 2 || directive_index == 3)
            {
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
            }
            else if (directive_index == 4)
            {
                int i; // dummy
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

PPToken *fetch_before_endif(PPToken *tok)
{
    if (tok == NULL)
    {
        return NULL;
    }
    while (tok)
    {
        if (tok->next->kind == PPTK_PUNC && *tok->next->str == '#' && isdirective_idx(tok->next->next, 4))
        {
            return tok;
        }
        tok = tok->next;
    }
    return NULL;
}

PPToken *preprocess_directives(char *base_dir, PPToken *tok)
{
    if (tok == NULL)
    {
        return tok;
    }
    PPToken *prev = tok;
    PPToken *cur = tok;

    while (cur)
    {
        if (cur->kind == PPTK_PUNC && *cur->str == '#')
        {
            // include
            if (cur->next->kind == PPTK_IDENT &&
                strncmp(cur->next->str, preprocessing_directive_list[0], strlen(preprocessing_directive_list[0])) == 0)
            {
                PPToken *hn_tok = cur->next->next;
                if (hn_tok->kind != PPTK_HN)
                {
                    error("不正なinclude文です");
                }
                char *p = hn_tok->str;             // < or "
                char *p_end = p + hn_tok->len - 1; // > or "
                char *file_name = calloc(1, hn_tok->len - 2);
                memcpy(file_name, p + 1, hn_tok->len - 2);
                PPToken *include_tok = NULL;
                if (*p == '"' && *p_end == '"')
                {
                    // とりあえず
                    if (strncmp("hooligan.h", file_name, strlen(file_name)))
                    {

                        char *full_path = join_str(base_dir, file_name);
                        char *dir = extract_dir(full_path);

                        include_tok = preprocess_directives(dir, decompose_to_pp_token(read_file(full_path)));
                    }
                }
                else
                {
                    char *full_path = join_str("include/", file_name);

                    PPToken *mid = decompose_to_pp_token(read_file(full_path));
                    include_tok = preprocess_directives("include/", mid);
                }
                if (include_tok == NULL)
                {
                    if (prev == cur)
                    {
                        prev = hn_tok->next;
                        cur = hn_tok->next;
                        tok = prev;
                    }
                    else
                    {
                        prev->next = hn_tok->next;
                        cur = hn_tok->next;
                    }
                    continue;
                }
                PPToken *include_tok_end = include_tok;
                while (include_tok_end->next)
                {
                    include_tok_end = include_tok_end->next;
                }

                include_tok_end->next = hn_tok->next;

                if (prev == cur)
                {
                    prev = include_tok_end;
                    cur = include_tok_end->next;
                    tok = include_tok;
                }
                else
                {
                    prev->next = include_tok;
                    prev = include_tok_end;
                    cur = include_tok_end->next;
                }
                continue;
            }

            // マクロの登録
            if (cur->next->kind == PPTK_IDENT &&
                strncmp(cur->next->str, preprocessing_directive_list[1], strlen(preprocessing_directive_list[1])) == 0)
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
                    tok = cur;
                }
                else
                {
                    prev->next = replace->next;
                    cur = replace->next;
                }
                continue;
            }
            // ifdef
            // strlen(preprocessing_directive_list[])を使わないと#ifの時にバグが出る
            if (cur->next->kind == PPTK_IDENT &&
                strncmp(cur->next->str, preprocessing_directive_list[2], strlen(preprocessing_directive_list[2])) == 0)
            {
                PPToken *target = cur->next->next;
                if (find_macro(target->str, target->len))
                {
                    PPToken *before_endif = fetch_before_endif(target);
                    if (before_endif == NULL)
                    {
                        error("ifdefの後にはendifが必要です");
                    }
                    PPToken *after_endif = before_endif->next->next->next;
                    before_endif->next = after_endif;
                    if (prev == cur)
                    {
                        prev = target->next;
                        cur = target->next;
                        tok = cur;
                    }
                    else
                    {
                        prev->next = target->next;
                        cur = target->next;
                    }
                }
                else
                {
                    PPToken *before_endif = fetch_before_endif(target);
                    if (before_endif == NULL)
                    {
                        error("ifdefの後にはendifが必要です");
                    }
                    PPToken *after_endif = before_endif->next->next->next;
                    if (prev == cur)
                    {
                        prev = after_endif;
                        cur = after_endif;
                        tok = cur;
                    }
                    else
                    {
                        prev->next = after_endif;
                        cur = after_endif;
                    }
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
