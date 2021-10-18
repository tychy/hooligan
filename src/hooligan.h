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

typedef enum
{
    PPTK_DUMMY, // #define identで使う
    PPTK_HN,    // header-name
    PPTK_IDENT,
    PPTK_NUMBER,
    PPTK_CHAR, // character-constant
    PPTK_STRING,
    PPTK_PUNC, // punctuators
} PPTokenKind;

// NOTE: 予約語を先頭に持ってくる
typedef enum
{
    TK_RETURN,
    TK_IF,
    TK_ELSE,
    TK_FOR,
    TK_WHILE,
    TK_SWITCH,
    TK_CASE,
    TK_DEFAULT,
    TK_SIZEOF,
    TK_VOID,
    TK_INT,
    TK_FLOAT,
    TK_CHAR,
    TK_STRUCT,
    TK_TYPEDEF,
    TK_BREAK,
    TK_CONTINUE,
    TK_STATIC,
    TK_EXTERN,
    TK_ENUM,
    TK_CONST,
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
    ND_FLOAT,
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_MOD,
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
    ND_SWITCH,
    ND_CASE,
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
    ND_POSTDEC,
    ND_NOP,
    ND_TYPE,
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
typedef struct Option Option;
typedef struct PPToken PPToken;
typedef struct Macro Macro;
typedef struct PPContext PPContext;
typedef struct Token Token;
typedef struct Type Type;
typedef struct Node Node;
typedef struct Var Var;
typedef struct String String;
typedef struct Float Float;
typedef struct Member Member;
typedef struct Scope Scope;
typedef struct Context Context;

struct Option
{
    bool is_verbose;
    int optimize_level;
};

struct PPToken
{
    PPTokenKind kind;
    PPToken *next;
    bool is_float;
    int val;
    float float_val;
    int len;
    char *str;

    int integer; // delete this
    int decimal; // delete this
    int numzero;
};

struct Macro
{
    PPToken *target;
    PPToken *replace;
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
    int value;
    float float_val;
    int length;
    char *string;

    int integer; // delete this
    int decimal; // delete this
    int numzero;
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
    NodeKind kind;
    int val;
    int offset;
    char *name;
    int length;
    Type *ty;

    // for gvar
    Node *gvar_init;

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
    // for switch-case
    Node *next_case;
    Node *default_case;
    // for variable
    bool is_local;
    // for static
    bool is_static;
    int scope_label;
    // for function
    int args_region_size;
    Node *args;
    int num_args;
    bool is_void;
    bool has_variable_length_arguments;

    // for float
    int data_label;

    // for string
    int strlabel;

    // for struct
    Member *member;

    // for break in switch
    int break_to;

    // labels
    int loop_label;             // for, while
    int cond_label;             // if, else
    int case_label;             // switch-case
    int logical_operator_label; // &&, ||

    Node *statements; // for block
    Node *next_stmt;  // for block children
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
    int num_args;
    Type *arg_ty_ls[6];
    bool has_variable_length_arguments;
    // for constant expression
    int value;
    // for const variable
    bool is_const;
};

struct String
{
    char *p;
    int length;
    int label;
    String *next;
};

struct Float
{
    float val;
    int label;
    Float *next;

    // delete this
    int integer;
    int decimal;
    int numzero;
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
    Var *constants;
    Type *defined_type; // for typedef
    Type *tagged_types; // for tag(e.x. struct)
    Scope *prev;
    Scope *next;
    Node *current_switch;
    int label;
    int loop_label; // for break and continue
    int break_to;
};

struct Context
{
    int scope_serial_num; // serial number for scope
    Scope *scope;
    String *strings;
    Float *floats;
    Var *statics;
    Var *functions;
    int data_label;
    int offset; // for local variable
    int break_to;
    int continue_to;
    int logical_operator_label;
    bool is_aligned_stack_ptr;
};

// Declaration of global variables
extern Option *opts;
extern PPContext *pp_ctx;
extern Token *token;
extern Node *nodes[500];
extern Context *ctx;
extern ILSentence *cur_ils;
extern FILE *output;

// Declaration of functions
// tokenizer.c
Token *tokenize(PPToken *pp_tok);

// preprcessor.c
PPToken *decompose_to_pp_token(char *p);
PPToken *preprocess_directives(char *base_dir, PPToken *tok);

// parser.c
void parse_program(Token *start);

// generator.c
ILSentence *gen_asm_intel();

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
#endif
