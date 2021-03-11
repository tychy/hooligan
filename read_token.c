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
    Type *ty = find_type(token);
    if (ty)
    {
        token = token->next;
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
    else
    {
        return NULL;
    }
    if (ty->ty == STRUCT && consume("{"))
    {
        int offset = 0;
        Member *head = calloc(1, sizeof(Member));
        Member *cur = head;
        while (not(consume("}")))
        {

            Member *mem = calloc(1, sizeof(Member));
            Type *mem_ty = consume_type();
            Token *mem_tok = consume_ident();
            if (consume("["))
            {
                int arr_size = expect_number();
                mem_ty = new_type_array(mem_ty, arr_size);
                expect("]");
            }

            mem->name = mem_tok->string;
            mem->length = mem_tok->length;
            mem->offset = offset;
            offset += calc_bytes(mem_ty);
            mem->ty = mem_ty;
            cur->next = mem;
            cur = mem;
            expect(";");
        }
        ty->members = head;
        ty->size = offset;
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
        error("'%c'ではありません", op[0]);
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
    if (token->kind != TK_CHAR)
    {
        error("文字ではありません");
        int value = token->value;
        token = token->next;
        return value;
    }
}

Token *consume_ident()
{
    if (token->kind != TK_IDENT)
    {
        error("変数ではありません");
    }
    Token *tok = token;
    token = token->next;
    return tok;
}

bool at_eof()
{
    return token->kind == TK_EOF;
}
