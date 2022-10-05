#pragma once

#include <stdint.h>
#include <stdlib.h>
#include "lexer.h"

// rest
typedef bool eot;
enum double_registers {
    A,
    B
};

enum single_registers {
    R1,
    R2,
    R3,
    R4
};

enum node_types {
    ADD,
    CMP,
    INP,
    JEQ,
    JMP,
    JNQ,
    JNZ,
    JZR,
    LDR,
    LOD,
    MOV,
    NAD,
    NOP,
    NOR,
    OUT,
    SUB,
    WTR
};

// you can just check if field & flag_to_check == field to see if a flag is set
#define USE_IMM_16 0b0001
#define USE_IMM_8  0b0010
#define USE_DREGS  0b0100
#define USE_REGS   0b1000

typedef struct {
    int config_field; // config bit field
    int reg1; // is always needed
    union {
        int reg2;
        uint8_t imm8;
    };
} add_instr_t;

typedef struct {
    int config_field; // config bit field
    int reg1; // is always needed
    union {
        int reg2;
        uint8_t imm8;
    };
} cmp_instr_t;

typedef struct {
    int reg1;
    int A;
} inp_instr_t;

typedef struct {
    int config_field;
    union {
        int dreg;
        int imm16;
    };
} jeq_instr_t;

typedef struct {
    int config_field;
    union {
        int dreg;
        int imm16;
    };
} jmp_instr_t;

typedef struct {
    int config_field;
    union {
        int dreg;
        int imm16;
    };
} jnq_instr_t;

typedef struct {
    int config_field;
    union {
        int dreg;
        int imm16;
    };
} jnz_instr_t;

typedef struct {
    int config_field;
    union {
        int dreg;
        int imm16;
    };
} jzr_instr_t;

typedef struct {
    int reg;
    int dreg;
} ldr_instr_t;

typedef struct {
    int reg;
    uint8_t imm8;
} lod_instr_t;

typedef struct {
    int reg1;
    int reg2;
} mov_instr_t;

typedef struct {
    int config_field;
    int reg1;
    union {
        int reg2;
        uint8_t imm8;
    };
} nad_instr_t;

typedef struct {
    int config_field; // make a enum for different configs. They can be | together to create the final config_field
    int reg1;
    union {
        int reg2;
        uint8_t imm8;
    };
} nor_instr_t;

typedef struct {
    int A;
    int reg;
} out_instr_t;

typedef struct {
    int reg1;
    union {
        int reg2;
        uint8_t imm8;
    };
} sub_instr_t;

typedef struct {
    int dreg;
    int reg;
} wtr_instr_t;

typedef struct {
    int node_type;
    union {
        add_instr_t add;
        cmp_instr_t cmp;
        inp_instr_t inp;
        jeq_instr_t jeq;
        jmp_instr_t jmp;
        jnq_instr_t jnq;
        jnz_instr_t jnz;
        jzr_instr_t jzr;
        ldr_instr_t ldr;
        lod_instr_t lod;
        mov_instr_t mov;
        nad_instr_t nad;
        // skip nop cause nop is special
        nor_instr_t nor;
        out_instr_t out;
        sub_instr_t sub;
        wtr_instr_t wtr;
    };
} node_t;

typedef struct {
    int value;
    char name[MAX_LABEL_SIZE];
} label_t;

typedef struct {
    node_t* nodes;
    label_t* labels;
    size_t number_of_nodes;
    size_t number_of_labels;
} root_t;

// functions
root_t* parse(token_t* tokens, size_t len, bool enable_errors);
void init_labels(token_t* tokens, size_t len);
node_t* dynamic_node(node_t* nodes, node_t node, size_t* number_of_nodes);
label_t* dynamic_label(label_t* labels, label_t label, size_t* number_of_labels);
void p_advance();
