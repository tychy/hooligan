#ifndef HOOLIGAN_H
#define HOOLIGAN_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "inter_language.h"

// list.c
typedef struct List List;
struct List
{
    void **elm;
    int size;
    int cap;
};
List *new_list(int cap);
List *append(List *li, void *val);

typedef enum
{
    PD_INCLUDE,
    PD_DEFINE,
    PD_IFDEF,
    PD_IFNDEF,
    PD_ENDIF,
    PD_LINE,
} PreprocessingDirective;

typedef enum
{
    RW_RETURN,
    RW_IF,
    RW_ELSE,
    RW_FOR,
    RW_WHILE,
    RW_SWITCH,
    RW_CASE,
    RW_DEFAULT,
    RW_SIZEOF,
    RW_VOID,
    RW_INT,
    RW_FLOAT,
    RW_CHAR,
    RW_STRUCT,
    RW_TYPEDEF,
    RW_BREAK,
    RW_CONTINUE,
    RW_STATIC,
    RW_EXTERN,
    RW_ENUM,
    RW_CONST,
} ReservedWord;

typedef enum
{
    TK_DUMMY,
    TK_PPDIRECTIVE,
    TK_HEADER_NAME,
    TK_NUMBER,
    TK_CHARACTER,
    TK_STRING,
    TK_OPERATOR,
    TK_RESERVED_WORD,
    TK_IDENT,
    TK_EOF,
} TokenKind;

typedef enum
{
    ND_NUM,
    ND_FLOAT,
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_MOD,
    ND_EQUAL,
    ND_GEQ,
    ND_GTH,
    ND_VAR,
    ND_ASSIGN,
    ND_RETURN,
    ND_IF,
    ND_FOR,
    ND_BLOCK,
    ND_WHILE,
    ND_SWITCH,
    ND_CASE,
    ND_FUNC,
    ND_FUNCDEF,
    ND_ADDR,
    ND_DEREF,
    ND_GVARDEF,
    ND_STRING,
    ND_MEMBER,
    ND_BREAK,
    ND_CONTINUE,
    ND_AND,
    ND_OR,
    ND_NOT,
    ND_POSTINC,
    ND_POSTDEC,
    ND_NOP,
    ND_TYPE,
    ND_ARRAY,
} NodeKind;

typedef enum
{
    VOID,
    INT,
    FLOAT,
    CHAR,
    PTR,
    ARRAY,
    STRUCT,
    ENUM,
} TypeKind;

// type definition
typedef struct Option Option;
typedef struct Macro Macro;
typedef struct PPContext PPContext;
typedef struct Token Token;
typedef struct Type Type;
typedef struct Node Node;
typedef struct Var Var;
typedef struct Member Member;
typedef struct Scope Scope;
typedef struct Context Context;

struct Option
{
    bool is_verbose;
    int optimize_level;
};

struct Macro
{
    Token *target;
    Token *replace;
    Macro *next;
};

struct PPContext
{
    Macro *macros;
};

struct Token
{
    TokenKind kind;
    Token *next;
    bool is_float;
    int val;
    float float_val;
    int len;
    char *str;
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
    Type *next_defined;

    // for struct
    char *tag_name;
    int tag_length;
    Type *next_tagged;

    // for const variable
    bool is_const;
};

struct Node
{
    int id;
    NodeKind kind;
    int val;
    int offset;
    char *name;
    int length;
    Type *ty;
    List *children;

    // for gvar
    Node *gvar_init;

    // for binary tree node
    Node *lhs;
    Node *rhs;
    // for single child node
    Node *child;

    // for(init; condition; on_end) body;
    Node *init;
    Node *condition;
    Node *on_end;
    Node *body;

    // if(condition) body else on_else
    Node *on_else;
    // for switch-case
    List *cases;
    Node *default_case;
    // for variable
    bool is_local;
    // for static
    bool is_static;
    // for function
    List *args;
    int args_region_size;
    bool is_type_prototyped;
    bool has_variable_length_arguments;

    // for float
    float f_val;

    // for string
    char *str_content;
    int str_len;

    // for struct
    Member *member;

    int break_to_id;    // for break
    int continue_to_id; // for continue

    List *statements;
    // for variable
    int variable_id;
};
struct Var
{
    int id;
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
    int num_args;
    Type *arg_ty_ls[6];
    bool has_variable_length_arguments;
    // for constant expression
    int value;
    bool is_computable;
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
    Type *defined_type; // for typedef
    Type *tagged_types; // for tag(e.x. struct)
    Scope *prev;
    Scope *next;
    Node *current_switch;
};

struct Context
{
    Scope *scope;
    int offset; // for local variable
    int break_to;
    int continue_to;
};

// Declaration of global variables
extern Option *opts;
extern PPContext *pp_ctx;
extern Context *ctx;
extern FILE *output;

// Declaration of functions
// tokenizer.c
Token *normalize_tokens(Token *pp_tok);

// preprcessor.c
Token *tokenize(char *p);
Token *preprocess_directives(char *base_dir, Token *tok);

// parser.c
Node **parse_program(Token *start);

// generator.c
ILProgram *generate_inter_language(Node **nodes);

// util.c
void error(char *fmt);
void error1(char *fmt, char *v1);
void error2(char *fmt, char *v1, char *v2);
void error_at(char *loc, char *msg);
char *insert_str(char *src, int pos, char *target);
char *join_str(char *pre, char *post);
char *read_file(char *path);
char *extract_filename(char *path);
char *extract_dir(char *path);
int f2bin(float x);
char *remove_extension(char *filename);
bool is_int(Type *ty);
bool is_float(Type *ty);
bool is_char(Type *ty);
bool is_int_or_char(Type *ty);
bool is_ptr(Type *ty);
int calc_bytes(Type *ty);

// TODO ここにlibhooligan.soのヘッダ情報を書いてしまっている。別途.hファイルを用意する必要あり
int h_strlen(char *c);
#endif
