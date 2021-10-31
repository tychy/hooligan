#include "hooligan.h"

#include "tokenizer/operator.c"
#include "tokenizer/reserved_word.c"
#include "tokenizer/preprocessing_directive.c"

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
                if (strncmp(p, directive, h_strlen(directive)) == 0)
                {
                    cur = new_token(TK_PPDIRECTIVE, cur, p);
                    cur->val = i;
                    cur->len += h_strlen(directive);
                    p += h_strlen(directive);
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

        if (isoperator(p))
        {
            for (int i = 0; i < operator_list_count; i++)
            {

                char *op = operator_list[i];
                if (strncmp(p, op, h_strlen(op)) == 0)
                {
                    cur = new_token(TK_OPERATOR, cur, op);
                    cur->len = h_strlen(op);
                    p += h_strlen(op);
                    break;
                }
            }
            continue;
        }

        if (isdigit(*p))
        {
            char *p_top = p;
            int val = 0;
            float power = 1.0;
            while (isdigit(*p))
            {
                val = 10 * val + (*p - '0');
                p++;
            }
            cur = new_token(TK_NUMBER, cur, p);
            cur->val = val;
            if (*p == '.')
            {
                char *remain;
                cur->float_val = strtof(p_top, &remain);
                p = remain;
                cur->is_float = true;
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

Token *normalize_tokens(Token *head)
{
    Token *cur = head;
    Token *before;
    while (cur != NULL)
    {
        if (cur->kind == TK_IDENT)
        {
            if (isreservedword(cur->str, cur->len))
            {
                cur->kind = TK_RESERVED_WORD;
                cur->val = find_reserved_word(cur->str, cur->len);
            }
        }
        before = cur;
        cur = cur->next;
    }
    before->next = calloc(1, sizeof(Token));
    before->next->kind = TK_EOF;
    return head;
}
