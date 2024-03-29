#ifndef INTER_LANGUAGE_H
#define INTER_LANGUAGE_H

#include <stdlib.h>

typedef enum
{
    ILOT_NUM,
    ILOT_REG,
    ILOT_PTR_TO_REG,
    ILOT_BYTE_PTR_TO_REG,
    ILOT_DWORD_PTR_TO_REG,
} ILOperandType;

typedef enum
{
    ILRG_RDI,
    ILRG_RSI,
    ILRG_RDX,
    ILRG_RCX,
    ILRG_R8,
    ILRG_R9,
    ILRG_RAX,
    ILRG_RBP,
    ILRG_RSP
} ILRegister;

typedef enum
{
    ILST_RAW,
    ILST_PUSH,
    ILST_POP,
} ILSentenceType;

typedef struct ILOperand ILOperand;   // rax, [rax], DWORD PTR [rax]など
typedef struct ILSentence ILSentence; // アセンブリコードの1行に相当する
typedef struct ILProgram ILProgram;

struct ILOperand
{
    ILOperandType ty;
    ILRegister reg;
    int val;
};

struct ILSentence
{
    ILSentenceType ty;
    ILOperand *first_operand;
    ILOperand *second_operand;
    char *raw_sentence;
    ILSentence *next;
};

struct ILProgram
{
    ILSentence *data;
    ILSentence *text;
};

ILOperand *new_il_operand_reg(ILRegister reg);
void generate_intel_syntax_assembly(ILProgram *program);

// optimizer.c
ILProgram *optimize_inter_language(ILProgram *program);

#endif
