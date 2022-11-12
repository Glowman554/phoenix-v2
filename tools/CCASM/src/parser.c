#include "include/parser.h"
#include "include/lexer.h"
#include "include/errors.h"
#include "include/utils.h"
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <string.h>

char* instructions[INSTRUCTIONS_LEN] = {"add", "cmp", "inp", "jeq", "jmp", "jnq", "jnz", "jzr", "lad", "ldr", "lod", "mov", "nad", "nop", "nor", "out", "sub", "wtr"};
parser_function corresponding_function[INSTRUCTIONS_LEN] = { register_register_or_imm8
                                                           , register_register_or_imm8
                                                           , register_A
                                                           , dregister_or_imm16
                                                           , dregister_or_imm16
                                                           , dregister_or_imm16
                                                           , dregister_or_imm16
                                                           , dregister_or_imm16
                                                           , dregister_imm16
                                                           , register_dregister
                                                           , register_imm8
                                                           , register_register
                                                           , register_register_or_imm8
                                                           , nop
                                                           , register_register_or_imm8
                                                           , A_register
                                                           , register_register_or_imm8
                                                           , dregister_register };


char* registers[REGISTER_LEN] = {"r1", "r2", "r3", "r4", "r5"};
char* d_registers[DREG_LEN] = {"A", "B"};

int instruction_count = 0; // offset by instr_count >> 24bits
int p_pos = -1;
eot is_eot = false;
token_t current_token;
root_t AST;
size_t len;
token_t* tokens;
bool enable_errors = true;

void init_labels(token_t* tokens, size_t len) {
    AST.labels = malloc(1*sizeof(label_t));
    parse(tokens, len, false);
    print_labels(AST.number_of_labels, AST.labels); // remove!
    //free(AST.nodes);
    AST.number_of_nodes = 0;
    p_pos = -1;
    is_eot = false;
    instruction_count = 0;
}

instruction_t* dynamic_node(instruction_t* nodes, instruction_t node, size_t* number_of_nodes) {
    nodes = realloc(nodes, ((*number_of_nodes)++)*sizeof(instruction_t));
    nodes[(*number_of_nodes)-1] = node;
    return nodes;
}
label_t* dynamic_label(label_t* labels, label_t label, size_t* number_of_labels) {
    labels = realloc(labels, ((*number_of_labels)++)*sizeof(label_t));
    labels[(*number_of_labels)-1] = label;
    return labels;
}

void p_advance() {
    if (p_pos+1 >= (int)len) is_eot = true;
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
    AST.nodes  = malloc(1*sizeof(instruction_t));
    AST.number_of_nodes  = 0;
    AST.number_of_labels = 0;
    p_advance();

    for (;!is_eot;) {
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
        
    } else if (p_pos+1 <= (int)len && tokens[p_pos+1].type == COLON) {
        // label
        label_t label_to_append = {.value = instruction_count};
        strcpy(label_to_append.name, current_token.string_data);
        AST.labels = dynamic_label(AST.labels, label_to_append, &AST.number_of_labels);
        p_advance();
        p_advance();
    } else {
        char err[33];
        sprintf(err, "Invalid instruction or label: \"%s\"", current_token.string_data);
        throw_error(err, enable_errors);
        p_advance();
    }
}

void* parse_second_class_id(int expected_type) {
    if (current_token.type == ID && get_label(AST.labels, &AST.number_of_labels, current_token.string_data).value >= 0) {
        if (expected_type != IMM_TYPE) throw_error("Type error", enable_errors);

        // get address of label
        label_t lbl = get_label(AST.labels, &AST.number_of_labels, current_token.string_data);
        return (void*)(&lbl.value);
    } else if (current_token.type == ID && IS_REGISTER) {
        if (expected_type != REG_TYPE) throw_error("Type error", enable_errors);

        // get register value
        int i = get_register();
        return (void*)(&i);
    } else if (current_token.type == ID && IS_DREG) {
        if (expected_type != DREG_TYPE) throw_error("Type error", enable_errors);

        // get dreg value
        for (int i = 16; i < DREG_LEN+16; i++) {
            if (strcmp(d_registers[i-16], current_token.string_data) == 0) return (void*)(&i);
        }
    } else {
        char err[36];
        sprintf(err, "Unexpected second class citizen: \"%s\"", current_token.string_data);
        throw_error(err, enable_errors);
        p_advance();
    }
}

int get_register() {
    for (int i = 1; i <= REGISTER_LEN; i++) {
        if (strcmp(registers[i-1], current_token.string_data) == 0) return i;
    }
}

bool is_instruction() {
    for (int i = 0; i < INSTRUCTIONS_LEN; i++) {
        if (strcmp(instructions[i], current_token.string_data) == 0) return true;
    }
    return false;
}

// will return a label with negative value if nothing was found
label_t get_label(label_t* labels, size_t* number_of_labels, const char* key) {
    for (int i = 0; i < (*number_of_labels); i++) {
        if (strcmp(labels[i].name, key) == 0) return labels[i];
    }
    label_t a = {.value = -1};
    return a;
}


// p functions
void register_register_or_imm8(void) {

}

void register_imm8(void) {

}

void register_A(void) {

}

void dregister_or_imm16(void) {

}

void dregister_imm16(void) {

}

void register_dregister(void) {

}

void register_register(void) {

}

void nop(void) {

}

void A_register(void) {

}

void dregister_register(void) {

}
