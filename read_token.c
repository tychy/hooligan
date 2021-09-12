#include "hooligan.h"

bool consume(char *op)
{
    if (token->kind != TK_OPERATOR || strcmp(token->string, op))
        return false;
    token = token->next;
    return true;
}

bool consume_rw(TokenKind tk)
{
    if (token->kind != tk)
        return false;
    token = token->next;
    return true;
}

Type *consume_type()
{
    Type *ty = find_defined_type(token);
    if (ty)
    {
        token = token->next;

        while (consume("*"))
        {
            ty = new_type_ptr(ty);
        }
        return ty;
    }
    if (consume_rw(TK_INT))
    {
        ty = new_type_int();
    }
    else if (consume_rw(TK_CHAR))
    {
        ty = new_type_char();
    }
    else if (consume_rw(TK_STRUCT))
    {
        ty = new_type_struct();
    }
    else if (consume_rw(TK_VOID))
    {
        ty = new_type_void();
    }
    else if (consume_rw(TK_ENUM))
    {
        ty = new_type_int();
        if (consume("{"))
        {
            int i = 0;
            while (!(consume("}")))
            {
                Token *ident = consume_ident();
                def_const(ident, i);
                i++;
                if (!(consume(",")))
                {
                    expect("}");
                    break;
                }
            }
            if (i == 0)
            {
                error("識別子が必要です");
            }
        }
        else
        {
            error("識別子が必要です");
        }
    }
    else
    {
        return NULL;
    }

    if (ty->ty == STRUCT)
    {
        Token *ident = consume_ident();
        if (consume("{"))
        {
            if (ident)
            {
                Type *tagged_type = find_tagged_type(ident);
                if (tagged_type)
                {
                    ty = tagged_type;
                }
                else
                {
                    add_tagged_type(ident, ty, true);
                }
            }
            set_struct_member(ty);
        }
        else if (ty->size == -1)
        {
            if (ident)
            {
                // typedef struct f ma;
                // struct f{ int x;}
                // struct f ma;
                Type *tagged_type = find_tagged_type(ident);
                if (tagged_type)
                {
                    ty = tagged_type;
                }
                else
                {
                    add_tagged_type(ident, ty, true);
                }
            }
        }
    }

    return ty;
}

Type *consume_ptr(Type *ty)
{
    if (!ty)
    {
        return ty;
    }

    while (consume("*"))
    {
        ty = new_type_ptr(ty);
    }
    return ty;
}

void expect(char *op)
{
    if (token->kind != TK_OPERATOR || token->string[0] != op[0])
    {
        error2("'%s'ではありません, got %s", op, token->string);
    }
    token = token->next;
}

int expect_number()
{
    if (token->kind != TK_NUMBER)
        error("数字ではありません");
    int value = token->value;
    token = token->next;
    return value;
}

int expect_char()
{
    if (token->kind != TK_CHARACTER)
    {
        error("文字ではありません");
    }
    int value = token->value;
    token = token->next;
    return value;
}

Token *consume_ident()
{
    if (token->kind != TK_IDENT)
    {
        return NULL;
    }
    Token *tok = token;
    token = token->next;
    return tok;
}

Token *expect_ident()
{
    if (token->kind != TK_IDENT)
    {
        error1("識別子ではありません got %s", token->string);
    }
    Token *tok = token;
    token = token->next;
    return tok;
}

bool at_eof()
{
    return token->kind == TK_EOF;
}
