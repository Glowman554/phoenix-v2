#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_INTEGER_SIZE 65536
#define MAX_LABEL_SIZE 0xff

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
        char string_data[MAX_LABEL_SIZE];
        uint16_t imm16_data;
    };
} token_t;

token_t* append_token(token_t* a, int* b, token_t c);
void advance();
token_t* lex(char* code, size_t code_len, int* number_of_tokens_produced);
token_t lex_number();
token_t lex_id();
