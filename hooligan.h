#ifndef HOOLIGAN_H
#define HOOLIGAN_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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
    TK_TYPEDEF,
    TK_BREAK,
    TK_CONTINUE,
    TK_STATIC,
    TK_EXTERN,
    // add reserved word above
    TK_OPERATOR,
    TK_NUMBER,
    TK_IDENT,
    TK_EOF,
    TK_STRING,
    TK_CHARACTER,
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
    ND_MEMBER,
    ND_BREAK,
    ND_CONTINUE,
    ND_AND,
    ND_OR,
    ND_NOT,
    ND_POSTINC,
} NodeKind;

typedef enum
{
    INT,
    CHAR,
    PTR,
    ARRAY,
    STRUCT,

} TypeKind;

typedef enum
{
    RG_RDI,
    RG_RSI,
    RG_RDX,
    RG_RCX,
    RG_R8,
    RG_R9,
    RG_RAX,
    RG_RBP,
    RG_RSP,
} RegisterName;

// type definition
typedef struct Token Token;
typedef struct Type Type;
typedef struct Node Node;
typedef struct Var Var;
typedef struct String String;
typedef struct Member Member;
typedef struct Scope Scope;
typedef struct Context Context;

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
    int size; // structで使う
    Member *members;

    // for defined type
    char *name;
    int length;
    Type *next;
};

struct Node
{
    NodeKind kind;
    int val;
    int offset;
    char *name;
    int length;
    Type *ty;

    // for binary tree node
    Node *lhs;
    Node *rhs;
    // for single child node
    Node *child;
    // for
    Node *next;

    // for(init; condition; on_end) body;
    Node *init;
    Node *condition;
    Node *on_end;
    Node *body;

    // if(condition) body else on_else
    Node *on_else;

    // for variable
    bool is_local;
    // for static
    bool is_static;
    int scope_label;
    // for function
    int args_region_size;
    Node *args;

    // for string
    int strlabel;

    //for struct
    Member *member;

    // labels
    int loop_label; // for, while
    int cond_label; // if, else
};
struct Var
{
    char *name;
    int length;
    int offset;
    int label;
    Type *ty;
    Var *next;
    bool is_local;
    bool is_typedef;
    // for static variable
    bool is_static;
    int init_val;
    // for function
    bool is_function;
};

struct String
{
    char *p;
    int length;
    int label;
    String *next;
};

struct Member
{

    Member *next;
    char *name;
    Type *ty;
    int length;
    int offset;
};

struct Scope
{
    Var *variables;
    Type *types;
    Scope *prev;
    Scope *next;
    int label;
    int loop_label; // for break and continue
};

struct Context{
    int scope_serial_num; // serial number for scope
    Scope *scope;
    String *strings;
    Var *statics;
    Var *functions;
    int offset; // for local variable
    int break_to;
    int continue_to;
};

// Declaration of global variables
extern Token *token;
extern Node *nodes[200];
extern Context *ctx;

// Declaration of functions
// read_token.c
bool consume(char *op);
bool consume_rw(TokenKind tk);
Type *consume_type();
void expect(char *op);
int expect_number();
int expect_char();
bool at_eof();
Token *consume_ident();

// tokenizer.c
Token *tokenize();

// parser.c
void program();

// generator.c
void gen_asm_intel();

// variable.c
Var *find_var(Token *tok);
Var *def_var(Token *tok, Type *ty, bool is_local);
Var *def_static_var(Token *tok, Type *ty, bool is_local, int init_val);
Var *find_func(Token *tok);
Var *def_func(Token *tok, Type *ty, bool is_static);

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
Type *def_type(Token *tok, Type *ty, bool is_local);
Type *find_type(Token *tok);

// scope.c
void new_scope();
void exit_scope();
void start_loop();
void end_loop();

// util.c
bool not(bool flag);
void error(char *fmt, ...);

#endif
