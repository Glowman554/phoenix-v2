#pragma once

#include <stdint.h>
#include <stdlib.h>
#include "lexer.h"
#include "../../../common/cpu.h"

// rest
enum {
    REG_TYPE,
    IMM_TYPE,
    DREG_TYPE
};

typedef bool eot;

#define IS_REGISTER (strcmp(current_token.string_data, "r1") == 0 || \
                     strcmp(current_token.string_data, "r2") == 0 || \
                     strcmp(current_token.string_data, "r3") == 0 || \
                     strcmp(current_token.string_data, "r4") == 0 || \
                     strcmp(current_token.string_data, "r5") == 0)

#define IS_DREG     (strcmp(current_token.string_data, "A") == 0 || \
                     strcmp(current_token.string_data, "B") == 0)

// parser functions
void register_register_or_imm8(void);
void register_imm8(void);
void register_A(void);
void dregister_or_imm16(void);
void dregister_imm16(void);
void register_dregister(void);
void register_register(void);
void nop(void);
void A_register(void);
void dregister_register(void);

typedef void (*parser_function)(void);

#define INSTRUCTIONS_LEN 18
extern char* instructions[INSTRUCTIONS_LEN];
extern parser_function corresponding_function[INSTRUCTIONS_LEN];

#define REGISTER_LEN 5
#define DREG_LEN 2
extern char* registers[REGISTER_LEN];
extern char* d_registers[DREG_LEN];

typedef struct {
    int id;
    char name[4];
} instruction_name;

typedef struct {
    int value;
    char name[MAX_LABEL_SIZE];
} label_t;

typedef struct {
    instruction_t* nodes;
    label_t* labels;
    size_t number_of_nodes;
    size_t number_of_labels;
} root_t;

// functions
root_t* parse(token_t* tokens, size_t len, bool enable_errors);
void init_labels(token_t* tokens, size_t len);
void parse_id();
void* parse_second_class_id(int expected_type);

instruction_t* dynamic_node(instruction_t* nodes, instruction_t node, size_t* number_of_nodes);
label_t* dynamic_label(label_t* labels, label_t label, size_t* number_of_labels);
void p_advance();

int get_register();
bool is_instruction();

label_t get_label(label_t* labels, size_t* number_of_labels, const char* key);
