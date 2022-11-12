#pragma once

#include "lexer.h"
#include "parser.h"
#include <stdio.h>

#define NOT_IMPLEMENTED printf("%s %s:%d is not implemented yet!\n", __func__, __FILE__, __LINE__);

void print_tokens(int token_len, token_t* tokens);
void print_labels(size_t label_len, label_t* labels);
bool in_bounds(char a, char b, char c);