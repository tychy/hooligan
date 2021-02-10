#ifndef HOOLIGAN_H
#define HOOLIGAN_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tokenizer
typedef enum
{
    TK_OPERATOR,
    TK_NUMBER,
    TK_IDENT,
    TK_EOL,
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

void error(char *fmt, ...);
bool consume(char *op);
void expect(char op);
int expect_number();
int expect_var();
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
} NodeKind;

typedef struct Node Node;

struct Node
{
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
    int offset;
};

Node *expr();
Node *stmt();
void gen(Node *node);

#endif
