#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_INTEGER16_SIZE 65536
#define MAX_INTEGER8_SIZE 255
#define MAX_LABEL_SIZE 0xff

typedef bool eof;

enum TokenTypes {
    ID,
    COLON,
    COMMA,
    NUMBER8,
    NUMBER16,
    LPAREN,
    RPAREN
};

typedef struct {
    int type;
    char string_data[MAX_LABEL_SIZE];
    uint16_t imm16_data;
} token_t;

token_t* append_token(token_t* a, int* b, token_t c);
void advance();
token_t* lex(char* code, size_t code_len, int* number_of_tokens_produced);
token_t lex_number();
token_t lex_id();
void print_tokens(int token_len, token_t* tokens);
bool in_bounds(char a, char b, char c);