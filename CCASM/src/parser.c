#include "include/parser.h"
#include "include/lexer.h"
#include "include/errors.h"
#include "include/utils.h"
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>

int instruction_count = 0; // offset by instr_count >> 24bits
int p_pos = -1;
eot is_eot = false;
token_t current_token;
root_t AST;
size_t len;
token_t* tokens;

void init_labels(token_t* tokens, size_t len) {
    AST.labels = malloc(1*sizeof(label_t));
    parse(tokens, len, false);
    print_labels(AST.number_of_labels, AST.labels); // remove!
    free(AST.nodes);
    AST.number_of_nodes = 0;
    p_pos = -1;
    is_eot = false;
    instruction_count = 0;
}

node_t* dynamic_node(node_t* nodes, node_t node, size_t* number_of_nodes) {
    nodes = realloc(nodes, ((*number_of_nodes)++)*sizeof(node_t));
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
root_t* parse(token_t* _tokens, size_t _len, bool enable_errors) {
    tokens = _tokens;
    len = _len;
    AST.nodes  = malloc(1*sizeof(node_t));
    AST.number_of_nodes  = 0;
    AST.number_of_labels = 0;
    p_advance();

    for (;!is_eot;) {
        // main switch for the start of instructions. Basically should all be IDs. Otherwise something went wrong horribly
        if (current_token.type == ID) {
            if (strcmp(current_token.string_data, "add") == 0) {
                // parse add
                
            } else if (strcmp(current_token.string_data, "cmp") == 0) {
                // parse cmp

            } else if (strcmp(current_token.string_data, "inp") == 0) {
                // parse inp

            } else if (strcmp(current_token.string_data, "jeq") == 0) {
                // parse jeq

            } else if (strcmp(current_token.string_data, "jmp") == 0) {
                // parse jmp

            } else if (strcmp(current_token.string_data, "jnq") == 0) {
                // parse jnq

            } else if (strcmp(current_token.string_data, "jnz") == 0) {
                // parse jnz

            } else if (strcmp(current_token.string_data, "jzr") == 0) {
                // parse jzr
                
            } else if (strcmp(current_token.string_data, "ldr") == 0) {
                // parse ldr

            } else if (strcmp(current_token.string_data, "lod") == 0) {
                // parse lod

            } else if (strcmp(current_token.string_data, "mov") == 0) {
                // parse mov

            } else if (strcmp(current_token.string_data, "nad") == 0) {
                // parse nad
                
            } else if (strcmp(current_token.string_data, "nop") == 0) {
                // parse nop
                instruction_count++;
                p_advance();
            } else if (strcmp(current_token.string_data, "nor") == 0) {
                // parse nor

            } else if (strcmp(current_token.string_data, "out") == 0) {

            } else if (strcmp(current_token.string_data, "sub") == 0) {
                // parse sub

            } else if (strcmp(current_token.string_data, "wtr") == 0) {
                // parse wtr

            } else {
                if (p_pos+1 <= (int)len && tokens[p_pos+1].type == COLON) {
                    // label
                    label_t label_to_append = {.value = instruction_count};
                    strcpy(label_to_append.name, current_token.string_data);
                    AST.labels = dynamic_label(AST.labels, label_to_append, &AST.number_of_labels);
                    p_advance();
                    p_advance();
                } else {
                    char err[24];
                    sprintf(err, "Invalid instruction: \"%s\"", current_token.string_data);
                    throw_error(err, enable_errors);
                    p_advance();
                }
            }
        } else {
            throw_error("Expected ID but got something else", enable_errors);
            p_advance();
        }
    }
    return &AST;
}
