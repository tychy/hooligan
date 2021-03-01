#ifndef HOOLIGAN_H
#define HOOLIGAN_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int label;
// Tokenizer
typedef enum
{
    TK_OPERATOR,
    TK_NUMBER,
    TK_IDENT,
    TK_RETURN,
    TK_IF,
    TK_ELSE,
    TK_FOR,
    TK_WHILE,
    TK_SIZEOF,
    TK_EOF,
} TokenKind;

typedef struct Token Token;

extern Token *token;

struct Token
{
    TokenKind kind;
    Token *next;
    int value;
    int length;
    char *string;
};

bool isident(char p);

void error(char *fmt, ...);
bool consume(char *op);
bool consume_return();
bool consume_if();
bool consume_else();
bool consume_for();
bool consume_while();
bool consume_sizeof();
void expect(char *op);
bool at_eof();
int expect_number();
Token *consume_ident();
Token *tokenize();

// Parser
typedef enum
{
    ND_NUM,
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_EQUAL,
    ND_NEQUAL,
    ND_GEQ,
    ND_LEQ,
    ND_GTH,
    ND_LTH,
    ND_LVAR,
    ND_ASSIGN,
    ND_RETURN,
    ND_IF,
    ND_FOR,
    ND_BLOCK,
    ND_WHILE,
    ND_FUNC,
    ND_ARG,
    ND_FUNCDEF,
    ND_ADDR,
    ND_DEREF,
    ND_GVARDEF,
} NodeKind;

typedef struct Type Type;
struct Type
{
    enum
    {
        INT,
        PTR,
        ARRAY
    } ty;
    Type *ptr_to;
    size_t array_size;
};

typedef struct Node Node;

extern Node *nodes[100];

struct Node
{
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
    int offset;
    char *name;
    int length;
    Type *ty;

    // for(init; condition; on_end) body;
    Node *init;
    Node *condition;
    Node *on_end;
    Node *body;

    // if(condition) body else on_else
    Node *on_else;

    // for function
    int args_region_size;
};

Node *expr();
Node *stmt();
void program();
void gen(Node *node);

typedef struct LVar LVar;

struct LVar
{
    char *name;
    int length;
    int offset;
    Type *ty;
    LVar *next;
};

extern LVar *locals;

LVar *find_lvar(Token *tok);
int def_lvar(Token *tok, Type *ty);
int calc_bytes(Type *ty);

#endif
