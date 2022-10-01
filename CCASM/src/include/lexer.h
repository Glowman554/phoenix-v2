#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_INTEGER_SIZE 65536

typedef bool eof;

enum TokenTypes {
    ID,
    COLON,
    COMMA,
    NUMBER,
    LPAREN,
    RPAREN
};

typedef struct {
    int type;
    union {
        char string_data[0xff];
        uint16_t imm16_data;
    };
} token_t;

token_t* append_token(token_t* a, size_t* b, token_t c);
void advance(int* pos, size_t* token_len, eof* is_eof);
token_t* lex(char* code, size_t code_len, size_t* number_of_tokens_produced);
token_t lex_number(char* code, size_t code_len, int* pos);
