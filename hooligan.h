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
    TK_EOF,
} TokenKind;

typedef struct Token Token;

Token *token;

struct Token
{
    TokenKind kind;
    Token *next;
    int value;
    char *string;
};

void error(char *fmt, ...);
bool consume(char *op);
void expect(char *op);
int expect_number();

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
} NodeKind;

typedef struct Node Node;

struct Node
{
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
    int length;
};

Node *expr();
void gen(Node *node);

#endif
