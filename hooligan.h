#ifndef HOOLIGAN_H
#define HOOLIGAN_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//utils
bool not(bool flag);

extern int label;
// enum
typedef enum
{
    RW_RETURN,
    RW_IF,
    RW_ELSE,
    RW_FOR,
    RW_WHILE,
    RW_SIZEOF,
} ReservedWord;
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
    ND_VAR,
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

typedef enum
{
    INT,
    CHAR,
    PTR,
    ARRAY,

} TypeKind;

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
bool istype(Token *tok, TypeKind ty);
int expect_number();
Token *consume_ident();
Token *tokenize();

// Parser
typedef struct Type Type;
struct Type
{
    TypeKind ty;
    Type *ptr_to;
    size_t array_size;
};

typedef struct Node Node;

extern Node *nodes[200];

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

    // for variable
    bool is_local;

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

void program();
void gen(Node *node);

typedef struct Var Var;
struct Var
{
    char *name;
    int length;
    int offset;
    Type *ty;
    Var *next;
    bool is_local;
};

extern Var *locals;
extern Var *globals;

Var *find_var(Token *tok, bool is_local);
int def_var(Token *tok, Type *ty, bool is_local);
int calc_bytes(Type *ty);

// type.c
Type *new_type_int();
Type *new_type_char();
Type *new_type_ptr(Type *ptr_to);
Type *new_type_array(Type *ptr_to, size_t size);
bool is_int(Type *ty);
bool is_char(Type *ty);
bool is_int_or_char(Type *ty);
Type *determine_expr_type(Type *lhs, Type *rhs);
#endif
