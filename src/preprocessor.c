#include "hooligan.h"

PPContext *pp_ctx;

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

static bool isdirective_idx(Token *tok, int idx)
{
    if (tok == NULL)
    {
        return false;
    }
    return (tok->kind == TK_PPDIRECTIVE && tok->val == idx);
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
        case TK_PPDIRECTIVE:
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
