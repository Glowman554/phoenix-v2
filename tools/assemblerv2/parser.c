#include<errors.h>
#include <lexer.h>
#include <parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>

char* instructions[INSTRUCTIONS_LEN] = {"add", "cmp", "inp", "jeq", "jmp", "jnq", "jnz", "jzr", "lad", "ldr", "lod", "mov", "nad", "nop", "nor", "out", "sub", "wtr"};
parser_function corresponding_function[INSTRUCTIONS_LEN] = {register_register_or_imm8, register_register_or_imm8, register_dregister, dregister_or_imm16, dregister_or_imm16, dregister_or_imm16, dregister_or_imm16, dregister_or_imm16, dregister_imm16, register_dregister, register_imm8, register_register, register_register_or_imm8, nop, register_register_or_imm8, dregister_register, register_register_or_imm8, dregister_register};
int instruction_opcodes[INSTRUCTIONS_LEN] = {INSTR_ADD, INSTR_CMP, INSTR_INPA, INSTR_JEQA, INSTR_JMPA, INSTR_JNQA, INSTR_JNZA, INSTR_JZRA, INSTR_LADA, INSTR_LDRA, INSTR_LOD, INSTR_MOV, INSTR_NAD, INSTR_NOP, INSTR_NOR, INSTR_OUTA, INSTR_SUB, INSTR_WTRA};

char* registers[REGISTER_LEN] = {"r0", "r1", "r2", "r3", "r4", "r5"};
uint8_t register_opcodes[REGISTER_LEN] = {R0, R1, R2, R3, R4, R5};
char* d_registers[DREG_LEN] = {"A", "B"};
uint8_t d_register_opcodes[DREG_LEN] = {A, B};

int instruction_count = 0; // offset by instr_count >> 24bits
int p_pos = -1;
eot is_eot = false;
token_t current_token;
root_t AST;
size_t len;
token_t* tokens;
bool enable_errors = true;

void init_labels(token_t* tokens, size_t len) {
	AST.labels = malloc(1 * sizeof(label_t));
	parse(tokens, len, false);
	// free(AST.nodes);
	AST.number_of_nodes = 0;
	p_pos = -1;
	is_eot = false;
	instruction_count = 0;
}

instruction_t* dynamic_node(instruction_t* nodes, instruction_t node, size_t* number_of_nodes) {
	*number_of_nodes = *number_of_nodes + 1;
	nodes = realloc(nodes, *number_of_nodes * sizeof(instruction_t));
	nodes[*number_of_nodes - 1] = node;
	return nodes;
}
label_t* dynamic_label(label_t* labels, label_t label, size_t* number_of_labels) {
	*number_of_labels = *number_of_labels + 1;
	labels = realloc(labels, *number_of_labels * sizeof(label_t));
	labels[*number_of_labels - 1] = label;
	return labels;
}

void p_advance() {
	if (p_pos + 1 >= (int)len)
		is_eot = true;
	else {
		p_pos++;
		current_token = tokens[p_pos];
	}
}

/*
- go through each token.
- make each valid instruction and check for errors
- get all labels
*/
root_t* parse(token_t* _tokens, size_t _len, bool _enable_errors) {
	enable_errors = _enable_errors;
	tokens = _tokens;
	len = _len;
	AST.nodes = malloc(1 * sizeof(instruction_t));
	AST.number_of_nodes = 0;
	p_advance();

	for (; !is_eot;) {
		// main switch for the start of instructions. Basically should all be IDs. Otherwise something went wrong horribly
		if (current_token.type == ID) {
			parse_id();
		} else {
			throw_error("Expected Instruction but got something else", enable_errors);
			p_advance();
		}
	}
	return &AST;
}

void parse_id() {
	if (is_instruction()) {
		for (int i = 0; i < INSTRUCTIONS_LEN; i++) {
			if (strcmp(instructions[i], current_token.string_data) == 0) {
				corresponding_function[i](instruction_opcodes[i]);
				instruction_count+=0x3;
			}
		}
	} else if (p_pos + 1 <= (int)len && tokens[p_pos + 1].type == COLON) {
		// label
		label_t label_to_append = {.value = instruction_count};
		strcpy(label_to_append.name, current_token.string_data);
		AST.labels = dynamic_label(AST.labels, label_to_append, &AST.number_of_labels);
		p_advance();
		p_advance();
	} else {
		char err[0xff];
		sprintf(err, "Invalid instruction or label: \"%s\"", current_token.string_data);
		throw_error(err, enable_errors);
		p_advance();
	}
}

int parse_second_class(int expected_type) {
	if (current_token.type == ID && get_label(AST.labels, &AST.number_of_labels, current_token.string_data).value >= 0) {
		if (expected_type != IMM16_TYPE)
			throw_error("Type error", enable_errors);

		// get address of label
		label_t lbl = get_label(AST.labels, &AST.number_of_labels, current_token.string_data);
		return lbl.value;
	} else if (current_token.type == ID && is_register(current_token.string_data)) {
		if (expected_type != REG_TYPE)
			throw_error("Type error", enable_errors);

		// get register value
		int i = get_register();
		i = register_opcodes[i];
		return i;
	} else if (current_token.type == ID && IS_DREG()) {
		if (expected_type != DREG_TYPE)
			throw_error("Type error", enable_errors);

		// get dreg value
		for (int i = 0; i < DREG_LEN; i++) {
			if (strcmp(d_registers[i], current_token.string_data) == 0) {
				return d_register_opcodes[i];
			}
		}
	} else if (current_token.type == NUMBER8) {
		if (expected_type == IMM16_TYPE) return current_token.imm16_data;
		if (expected_type != IMM8_TYPE)
			throw_error("Type error", enable_errors);
		return current_token.imm16_data;
	} else if (current_token.type == NUMBER16) {
		if (expected_type != IMM16_TYPE && expected_type != NUMBER8)
			throw_error("Type error", enable_errors);
		return current_token.imm16_data;
	} else {
		char err[0xff];
		sprintf(err, "Unexpected second class citizen: \"%s\"", current_token.string_data);
		throw_error(err, enable_errors);
		p_advance();
	}
	return -1;
}

int get_register() {
	for (int i = 0; i < REGISTER_LEN; i++) {
		if (strcmp(registers[i], current_token.string_data) == 0)
			return i;
	}
	return -1;
}

bool is_instruction() {
	for (int i = 0; i < INSTRUCTIONS_LEN; i++) {
		if (strcmp(instructions[i], current_token.string_data) == 0)
			return true;
	}
	return false;
}

// will return a label with negative value if nothing was found
label_t get_label(label_t* labels, size_t* number_of_labels, const char* key) {
	for (int i = 0; i < (*number_of_labels); i++) {
		if (strcmp(labels[i].name, key) == 0)
			return labels[i];
	}
	label_t a = {.value = -1};
	return a;
}

// p functions
void register_register_or_imm8(int opcode) {
	instruction_t instr = {.opcode = opcode};

	// if intermediate (imm8 instead of reg) incr opcode by 0x1
	p_advance();

	// register 1
	instr.reg1 = parse_second_class(REG_TYPE);
	p_advance();

	parse_comma();

	if (is_register(current_token.string_data)) {
		instr.reg2 = parse_second_class(REG_TYPE);
		p_advance();
	} else {
		instr.imm = parse_second_class(IMM8_TYPE);
		instr.opcode = get_opcode_increment_from_opcode(instr.opcode);
		// instr.opcode += 0x1;
		p_advance();
	}

	APPEND_TO_AST();
}

void register_imm8(int opcode) {
	instruction_t instr = {.opcode = opcode};
	p_advance();

	instr.reg1 = parse_second_class(REG_TYPE);
	p_advance();

	parse_comma();

	instr.imm = parse_second_class(IMM8_TYPE);
	p_advance();

	APPEND_TO_AST();
}

void dregister_or_imm16(int opcode) {
	// if dreg is b. Add 0x5
	instruction_t instr = {.opcode = opcode};
	p_advance();

	if (IS_DREG()) {
		int i = parse_second_class(DREG_TYPE);
		INCR_ON_DREG(i);
		p_advance();
	} else {
		instr.imm16 = parse_second_class(IMM16_TYPE);
		p_advance();
		// call get opcode increment to get for example jnqi.
		instr.opcode = get_opcode_increment_from_opcode(instr.opcode);
	}

	APPEND_TO_AST();
}

void dregister_imm16(int opcode) {
	// if dreg is b. Add 0x5
	instruction_t instr = {.opcode = opcode};
	p_advance();

	int i = parse_second_class(DREG_TYPE);
	INCR_ON_DREG(i);
	p_advance();

	parse_comma();

	instr.imm16 = parse_second_class(IMM16_TYPE);
	p_advance();

	APPEND_TO_AST();
}

void register_dregister(int opcode) {
	// if dreg is b. Add 0x5
	instruction_t instr = {.opcode = opcode};
	p_advance();

	instr.reg1 = parse_second_class(REG_TYPE);
	p_advance();

	parse_comma();

	int i = parse_second_class(DREG_TYPE);
	INCR_ON_DREG(i);
	p_advance();

	APPEND_TO_AST();
}

void register_register(int opcode) {
	instruction_t instr = {.opcode = opcode};
	p_advance();

	instr.reg1 = parse_second_class(REG_TYPE);
	p_advance();

	parse_comma();

	instr.reg2 = parse_second_class(REG_TYPE);
	p_advance();

	APPEND_TO_AST();
}

void nop(int opcode) {
	instruction_t instr = {.opcode = opcode};
	p_advance();
	APPEND_TO_AST();
}

void dregister_register(int opcode) {
	// if dreg is b. Add 0x5
	instruction_t instr = {.opcode = opcode};
	p_advance();

	int i = parse_second_class(DREG_TYPE);
	INCR_ON_DREG(i);
	p_advance();

	parse_comma();

	instr.reg1 = parse_second_class(REG_TYPE);
	p_advance();

	APPEND_TO_AST();
}

void parse_comma() {
	if (IS_COMMA())
		p_advance();
	else
		throw_error("Comma expected!", enable_errors);
}

bool is_register(char* cmp) {
	for (int i = 0; i < REGISTER_LEN;i++) {
		if (strcmp(registers[i], cmp) == 0) return true;
	}
	return false;
}

int get_opcode_increment_from_opcode(int x) {
	// switch to get all the I functions
	switch (x) {
	case INSTR_ADD:
		return INSTR_ADDI;
		break;
	case INSTR_CMP:
		return INSTR_CMPI;
		break;
	case INSTR_JEQA:
		return INSTR_JEQI;
		break;
	case INSTR_JMPA:
		return INSTR_JMPI;
		break;
	case INSTR_JNQA:
		return INSTR_JNQI;
		break;
	case INSTR_JNZA:
		return INSTR_JNZI;
		break;
	case INSTR_JZRA:
		return INSTR_JZRI;
		break;
	case INSTR_NAD:
		return INSTR_NADI;
		break;
	case INSTR_NOR:
		return INSTR_NORI;
		break;
	case INSTR_SUB:
		return INSTR_SUBI;
		break;
	default:
		return x;
		break;
	}
	return x;
}

int get_opcode_increment_from_for_b(int x) {
	// switch to get all B functions from A
	switch (x) {
	case INSTR_INPA:
		return INSTR_INPB;
		break;
	case INSTR_JEQA:
		return INSTR_JEQB;
		break;
	case INSTR_JMPA:
		return INSTR_JMPB;
		break;
	case INSTR_JNQA:
		return INSTR_JNQB;
		break;
	case INSTR_JNZA:
		return INSTR_JNZB;
		break;
	case INSTR_LADA:
		return INSTR_LADB;
		break;
	case INSTR_LDRA:
		return INSTR_LDRB;
		break;
	case INSTR_OUTA:
		return INSTR_OUTB;
		break;
	case INSTR_WTRA:
		return INSTR_WTRB;
		break;
	default:
		return x;
		break;
	}
	return x;
}
