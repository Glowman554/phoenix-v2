#include "include/lexer.h"
#include "include/errors.h"
#include <stdio.h>

void advance(int* pos, size_t* token_len, eof* is_eof) {
    if ((*pos) > (int)(*token_len)) ++*is_eof;
    else ++*pos;
}

token_t lex_number(char* code, size_t code_len, int* pos) {
    eof is_eof = false;
    int base = 10;
    int start_range1 = '0';
    int end_range1   = '9';
    int start_range2 = 'a';
    int end_range2   = 'f';

    if (code[*pos] == '0') {
        if (*pos+1 <= (int)code_len && code[*pos+1] == 'x') {
            // base 16
            advance(pos, &code_len, &is_eof);
            advance(pos, &code_len, &is_eof);
            base = 16;
        } else if (*pos+1 <= (int)code_len && code[*pos+1] == 'b') {
            // base 2
            advance(pos, &code_len, &is_eof);
            advance(pos, &code_len, &is_eof);
            base = 2;
            start_range1 = '0';
            end_range1   = '1';
            start_range2 = start_range1;
            end_range2 = end_range1;
        } else {
            // base 10
            base = 10;
            start_range2 = start_range1;
            end_range2 = end_range1;
        }
    } else {
        // base 10
        base = 10;
        start_range2 = start_range1;
        end_range2 = end_range1;
    }
    int number = 0;
    for (;!is_eof && (code[*pos] >= start_range1 && code[*pos] <= end_range1) && (code[*pos] >= start_range2 && code[*pos] <= end_range2); advance(pos, &code_len, &is_eof)) {
        number += (code[*pos] - start_range2) * base;
    }

    if (number > MAX_INTEGER_SIZE) {
        char str[31];
        sprintf(str, "Integer to big. Is %d. Max is %d", number, MAX_INTEGER_SIZE);
        throw_error(str, true);
    }
    token_t ret_tok = {.imm16_data = number, .type = NUMBER};
    return ret_tok;
}


// realloc tokens and append
token_t* append_token(token_t* a, size_t* b, token_t c) {
    if (*b == 0) {
        a[0] = c;
    } else {
        a = realloc(a, (*++b)*sizeof(token_t));
        a[*b] = c;
    }
    return a;
}

token_t* lex(char* code, size_t code_len, size_t* number_of_tokens_produced) {
    eof is_eof = false;
    token_t* tokens = malloc(sizeof(token_t));

    for (int pos = 0; !is_eof; advance(&pos, &code_len, &is_eof)) {
        char current_char = code[pos];
        switch (current_char) {
            case ' ':  break;
            case '\n': break;
            case '\t': break;

            case ':':
            {
                token_t to_append = {.type = COLON, .string_data = ":"};
                tokens = append_token(tokens, number_of_tokens_produced, to_append);
            } break;
            case '(':
            {
                token_t to_append = {.type = LPAREN, .string_data = "("};
                tokens = append_token(tokens, number_of_tokens_produced, to_append);
            } break;
            case ')':
            {
                token_t to_append = {.type = RPAREN, .string_data = ")"};
                tokens = append_token(tokens, number_of_tokens_produced, to_append);
            } break;

            default:
            {
                // check for IDs and Numbers. otherwise throw error
                if (current_char == '_' || (current_char >= 'a' && current_char <= 'z') || (current_char >= 'A' && current_char <= 'Z')) {
                    // ID

                } else if (current_char >= '0' && current_char <= '9')  {
                    // Number
                    tokens = append_token(tokens, number_of_tokens_produced, lex_number(code, code_len, &pos));
                } else {
                    char str[17];
                    sprintf(str, "Unknown Token: %d", current_char);
                    throw_error(str, true);
                }
            } break;
        }
    }
    return tokens;
}