#pragma once

#include <cpu.h>
#include <lexer.h>
#include <stdint.h>
#include <stdlib.h>

// rest
enum { REG_TYPE, IMM16_TYPE, IMM8_TYPE, DREG_TYPE };

typedef bool eot;

bool is_register(char* cmp);

#define IS_DREG() (strcmp(current_token.string_data, "A") == 0 || strcmp(current_token.string_data, "B") == 0)

#define IS_COMMA() (current_token.type == COMMA)
#define INCR_ON_DREG(i) if (i == B) instr.opcode = get_opcode_increment_from_for_b(instr.opcode);
void append_to_ast(instruction_t instr);

// parser functions
void register_or_imm8(int opcode);
void only_register(int opcode);
void only_dregister(int opcode);
void only_imm8(int opcode);
void register_register_or_imm8(int opcode);
void register_imm8(int opcode);
void dregister_or_imm16(int opcode);
void dregister_imm16(int opcode);
void register_dregister(int opcode);
void register_register(int opcode);
void nop(int opcode);
void dregister_register(int opcode);
void db(int opcode);

typedef void (*parser_function)(int);

#define INSTRUCTIONS_LEN 33
extern char* instructions[INSTRUCTIONS_LEN];
extern parser_function corresponding_function[INSTRUCTIONS_LEN];
extern int instruction_opcodes[INSTRUCTIONS_LEN];

#define REGISTER_LEN 16
#define DREG_LEN 2
extern char* registers[REGISTER_LEN];
extern uint8_t register_opcodes[REGISTER_LEN];
extern char* d_registers[DREG_LEN];
extern uint8_t d_register_opcodes[DREG_LEN];

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
int parse_second_class(int expected_type);
void parse_comma();

instruction_t* dynamic_node(instruction_t* nodes, instruction_t node, size_t* number_of_nodes);
label_t* dynamic_label(label_t* labels, label_t label, size_t* number_of_labels);
void p_advance();

int get_register();
bool is_instruction();

label_t get_label(label_t* labels, size_t* number_of_labels, const char* key);
int get_opcode_increment_from_opcode(int x);
int get_opcode_increment_from_for_b(int x);
