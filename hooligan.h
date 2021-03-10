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

// NOTE: 予約語を先頭に持ってくる
typedef enum
{
    TK_RETURN,
    TK_IF,
    TK_ELSE,
    TK_FOR,
    TK_WHILE,
    TK_SIZEOF,
    TK_INT,
    TK_CHAR,
    TK_STRUCT,
    // add reserved word above
    TK_OPERATOR,
    TK_NUMBER,
    TK_IDENT,
    TK_EOF,
    TK_STRING,
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
    ND_STRING,
    ND_INIT,
} NodeKind;

typedef enum
{
    INT,
    CHAR,
    PTR,
    ARRAY,
    STRUCT,

} TypeKind;

// type definition
typedef struct Token Token;
typedef struct Type Type;
typedef struct Node Node;
typedef struct Var Var;
typedef struct Vec Vec;

struct Token
{
    TokenKind kind;
    Token *next;
    int value;
    int length;
    char *string;
};
struct Type
{
    TypeKind ty;
    Type *ptr_to;
    size_t array_size;
};
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

    // for string
    int strlabel;
};
struct Var
{
    char *name;
    int length;
    int offset;
    Type *ty;
    Var *next;
    bool is_local;
};

struct Vec
{

    char *p;
    int length;
    int label;
    Vec *next;
};
// Declaration of global variables
extern int label;
extern Token *token;
extern Node *nodes[200];
extern Var *locals;
extern Var *globals;
extern Vec *strings;
// Declaration of functions
// read_token.c
void error(char *fmt, ...); // これutilのほうがいい
bool consume(char *op);
bool consume_rw(TokenKind tk);
Type *consume_type();
void expect(char *op);
bool at_eof();

// parser.c
int expect_number();
Token *consume_ident();
Token *tokenize();
void program();
void gen(Node *node);

// variable.c
Var *find_var(Token *tok, bool is_local);
Var *def_var(Token *tok, Type *ty, bool is_local);

// type.c
Type *new_type_int();
Type *new_type_char();
Type *new_type_string();
Type *new_type_ptr(Type *ptr_to);
Type *new_type_array(Type *ptr_to, size_t size);
Type *new_type_struct();
bool is_int(Type *ty);
bool is_char(Type *ty);
bool is_int_or_char(Type *ty);
int calc_bytes(Type *ty);
Type *determine_expr_type(Type *lhs, Type *rhs);

#endif
