#include "hooligan.h"

PPContext *pp_ctx;

// note: 文字数の多いものを先に登録する
// note: 要素数を更新する
static char *punctuator_list[35] = {
    "...",
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

static int punctuator_list_count = 35;

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

static Token *new_token(TokenKind kind, Token *cur, char *str)
{
    Token *tok = calloc(1, sizeof(Token));
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

static bool isdirective_idx(Token *tok, int idx)
{
    if (tok == NULL)
    {
        return false;
    }
    return (tok->kind == TK_IDENT &&
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
    else if (p == '\"')
    {
        return '\"';
    }
    else
    {
        printf("%c ", p);
        error("未定義のエスケープ文字です");
    }
}

Token *tokenize(char *p)
{
    Token head;
    head.next = NULL;
    Token *cur = &head;
    int line = 1;

    while (*p)
    {
        if (*p == '\n')
        {
            p++;
            line++;
            continue;
        }

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
                error_at(p, "コメントが閉じられていません");
            p = q + 2;
            continue;
        }

        // プリプロセッシング命令文の処理、暴力的な長さなので切り出しを検討すべき
        if (strncmp(p, "#", 1) == 0)
        {
            cur = new_token(TK_OPERATOR, cur, p);
            cur->len = 1;
            p++;
            if (!isdirective(p))
            {
                error_at(p, "未定義のプリプロセッシング命令文です");
            }
            int directive_index = -1;
            for (int i = 0; i < preprocessing_directive_list_count; i++)
            {
                char *directive = preprocessing_directive_list[i];
                if (strncmp(p, directive, strlen(directive)) == 0)
                {
                    cur = new_token(TK_IDENT, cur, p);
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
                            cur = new_token(TK_HEADER_NAME, cur, p - len);
                            cur->len = len + 1;
                            break;
                        }
                        len++;
                        p++;
                    }
                    if (cur->kind != TK_HEADER_NAME)
                    {
                        error_at(p, "不正なinclude文です");
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
                            cur = new_token(TK_HEADER_NAME, cur, p - len);
                            cur->len = len + 1;
                            break;
                        }
                        len++;
                        p++;
                    }
                    if (cur->kind != TK_HEADER_NAME)
                    {
                        error_at(p, "不正なinclude文です");
                    }
                }
                else
                {
                    error_at(p, "不正なinclude文です");
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
                cur = new_token(TK_IDENT, cur, p_top);
                cur->len = i;
                if (*p == '\n')
                {
                    // #define identの場合
                    cur = new_token(TK_DUMMY, cur, "");
                    line++;
                    continue;
                }

                while (isspace(*p))
                {
                    p++;
                    if (*p == '\n')
                    {
                        // #define identの場合
                        cur = new_token(TK_DUMMY, cur, "");
                        line++;
                        break;
                    }
                }
                if (cur->kind == TK_DUMMY)
                {
                    continue;
                }

                if (isnondigit(*p))
                {
                    i = 0;
                    p_top = p;
                    while (isnondigit(*p) || isdigit(*p))
                    {
                        i++;
                        p++;
                    }
                    cur = new_token(TK_IDENT, cur, p_top);
                    cur->len = i;
                }
                else if (isdigit(*p))
                {
                    cur = new_token(TK_NUMBER, cur, p);
                    cur->val = strtol(p, &p, 10);
                }
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
                cur = new_token(TK_IDENT, cur, p_top);
                cur->len = i;
            }
            else if (directive_index == 4)
            {
                int i; // dummy
            }
            else if (directive_index == 5)
            {
                // #line 10
                while (isspace(*p))
                {
                    p++;
                }
                if (isdigit(*p))
                {
                    cur = new_token(TK_NUMBER, cur, p);
                    cur->val = strtol(p, &p, 10);
                    line = cur->val - 1;
                }
                else
                {
                    error("不正なline文です");
                }
            }
            else
            {
                error_at(p, "未定義のプリプロセッシング命令文です");
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
                    if (!*(p + 1))
                    {
                        error_at(p, "エスケープ文字のあとに文字がありません");
                    }
                    p += 2;
                    i += 2;
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
                error_at(p, "ダブルクォテーションが閉じていません");

            p++;
            cur = new_token(TK_STRING, cur, p_top);
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
            cur = new_token(TK_CHARACTER, cur, p);
            cur->val = val;
            p++;
            if (*p != '\'')
            {
                error_at(p, "シングルクォーテーションが閉じていません");
            }
            p++;
            continue;
        }

        if (isnondigit(*p))
        {
            if (strncmp(p, "__LINE__", 8) == 0)
            {
                cur = new_token(TK_NUMBER, cur, p);
                cur->val = line;
                p += 8;
                continue;
            }

            int i = 0;
            char *p_top = p;
            while (isnondigit(*p) || isdigit(*p))
            {
                i++;
                p++;
            }
            cur = new_token(TK_IDENT, cur, p_top);
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
                    cur = new_token(TK_OPERATOR, cur, op);
                    cur->len = strlen(op);
                    p += strlen(op);
                    break;
                }
            }
            continue;
        }

        if (isdigit(*p))
        {
            int val = 0;
            float float_val = 0.0;

            float power = 1.0;
            while (isdigit(*p))
            {
                val = 10 * val + (*p - '0');
                float_val = 10.0 * float_val + (*p - '0');
                p++;
            }
            cur = new_token(TK_NUMBER, cur, p);
            cur->val = val;
            if (*p == '.')
            {
                p++;

                char *decimal_head; // delete this
                decimal_head = p;   // delete this
                while (isdigit(*p))
                {
                    float_val = 10.0 * float_val + (*p - '0');
                    power *= 10.0;
                    p++;
                }
                cur->is_float = true;
                cur->float_val = float_val / power;
                // ここからあとはアドホックな実装
                int decimal = 0;
                int numzero = 0;
                while (*decimal_head == '0')
                {
                    numzero++;
                    decimal_head++;
                }

                while (isdigit(*decimal_head))
                {
                    decimal = 10 * decimal + (*decimal_head - '0');
                    decimal_head++;
                }
                cur->integer = val;
                cur->decimal = decimal;
                cur->numzero = numzero;
            }
            else
            {
                cur->is_float = false;
            }

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

Token *fetch_before_endif(Token *tok)
{
    if (tok == NULL)
    {
        return NULL;
    }
    while (tok)
    {
        if (tok->next->kind == TK_OPERATOR && *tok->next->str == '#' && isdirective_idx(tok->next->next, 4))
        {
            return tok;
        }
        tok = tok->next;
    }
    return NULL;
}

Token *preprocess_directives(char *base_dir, Token *tok)
{
    if (tok == NULL)
    {
        return tok;
    }
    Token *prev = tok;
    Token *cur = tok;

    while (cur)
    {
        if (cur->kind == TK_OPERATOR && *cur->str == '#')
        {
            // include
            if (isdirective_idx(cur->next, 0))
            {
                Token *hn_tok = cur->next->next;
                if (hn_tok->kind != TK_HEADER_NAME)
                {
                    error_at(cur->str, "不正なinclude文です");
                }
                char *p = hn_tok->str;             // < or "
                char *p_end = p + hn_tok->len - 1; // > or "
                char *file_name = calloc(1, hn_tok->len - 1);
                memcpy(file_name, p + 1, hn_tok->len - 2);
                Token *include_tok = NULL;
                if (*p == '"' && *p_end == '"')
                {

                    char *full_path = join_str(base_dir, file_name);
                    char *dir = extract_dir(full_path);

                    include_tok = preprocess_directives(dir, tokenize(read_file(full_path)));
                }
                else
                {
                    char *full_path = join_str("include/", file_name);

                    Token *mid = tokenize(read_file(full_path));
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
                Token *include_tok_end = include_tok;
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
            if (isdirective_idx(cur->next, 1))
            {
                Token *target = cur->next->next;
                Token *replace = cur->next->next->next;

                if (target->kind == TK_IDENT && (replace->kind == TK_IDENT || replace->kind == TK_NUMBER))
                {
                    if (target->len == replace->len && strncmp(target->str, replace->str, target->len) == 0)
                    {
                        error_at(cur->str, "target == replace　のマクロは定義できません");
                    }
                    if (find_macro(target->str, target->len) != NULL)
                    {
                        error_at(cur->str, "マクロの二重定義です");
                    }
                }
                else if (target->kind == TK_IDENT && replace->kind == TK_DUMMY)
                {
                    // #define identに対応
                    if (find_macro(target->str, target->len) != NULL)
                    {
                        error_at(cur->str, "マクロの二重定義です");
                    }
                }
                else
                {
                    error_at(cur->str, "不正なdefine文です");
                }
                Macro *mac = calloc(1, sizeof(Macro));
                mac->target = target;
                mac->replace = replace;
                mac->next = pp_ctx->macros;
                pp_ctx->macros = mac;

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
            if (isdirective_idx(cur->next, 2))
            {
                Token *target = cur->next->next;
                if (find_macro(target->str, target->len))
                {
                    Token *before_endif = fetch_before_endif(target);
                    if (before_endif == NULL)
                    {
                        error_at(cur->str, "ifdefの後にはendifが必要です");
                    }
                    Token *after_endif = before_endif->next->next->next;
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
                    Token *before_endif = fetch_before_endif(target);
                    if (before_endif == NULL)
                    {
                        error_at(cur->str, "ifdefの後にはendifが必要です");
                    }
                    Token *after_endif = before_endif->next->next->next;
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
            // ifndef
            if (isdirective_idx(cur->next, 3))
            {
                Token *target = cur->next->next;
                if (!find_macro(target->str, target->len))
                {
                    Token *before_endif = fetch_before_endif(target);
                    if (before_endif == NULL)
                    {
                        error_at(cur->str, "ifndefの後にはendifが必要です");
                    }
                    Token *after_endif = before_endif->next->next->next;
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
                    Token *before_endif = fetch_before_endif(target);
                    if (before_endif == NULL)
                    {
                        error_at(cur->str, "iffdefの後にはendifが必要です");
                    }
                    Token *after_endif = before_endif->next->next->next;
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
            if (isdirective_idx(cur->next, 5))
            {
                cur = cur->next->next->next;
                prev->next = cur;
            }
        }
        // マクロの検索
        if (cur->kind == TK_IDENT)
        {
            for (;;)
            {
                Macro *mac = find_macro(cur->str, cur->len);
                if (mac)
                {
                    cur->kind = mac->replace->kind;
                    cur->str = mac->replace->str;
                    cur->len = mac->replace->len;
                    cur->val = mac->replace->val;
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

void dump_pp_token(Token *tok)
{
    Token *cur = tok;
    while (cur)
    {
        switch (cur->kind)
        {
        case TK_CHARACTER:
            printf("%d ", cur->val);
            break;
        case TK_HEADER_NAME:
            printf("%.*s ", cur->len, cur->str);
            break;
        case TK_IDENT:
            printf("%.*s ", cur->len, cur->str);
            break;
        case TK_NUMBER:
            if (cur->is_float)
            {
                printf("%f ", cur->float_val);
                break;
            }
            else
            {
                printf("%d ", cur->val);
                break;
            }
        case TK_OPERATOR:
            printf("%.*s ", cur->len, cur->str);
            break;
        case TK_STRING:
            printf("\\\"%.*s\\\" ", cur->len, cur->str);
            break;
        }
        cur = cur->next;
    }
    printf("\n");
}
