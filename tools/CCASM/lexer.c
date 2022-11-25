#include <lexer.h>
#include <errors.h>
#include <utils.h>
#include <stdio.h>
#include <string.h>

int pos = -1;
size_t code_len = 0;
char* code;
char current_char = '\0';

void advance() {
    if (pos > (int)code_len) current_char = '\0';
    else {
        pos++;
        current_char = code[pos];
    }
}

token_t lex_id() {
    token_t t = {.type = ID};
    int start_pos = pos;
    while (current_char != '\0' && (in_bounds(current_char, 'a', 'z') || in_bounds(current_char, 'A', 'Z') || in_bounds(current_char, '0', '9') || current_char == '_')) {
        if ((pos - start_pos) >= (MAX_LABEL_SIZE-1)) {
            // error. Label to long
            throw_error("Label to long!", true);
        }
        t.string_data[pos - start_pos] = current_char;
        advance();
    }
    t.string_data[pos-start_pos] = '\0';
    return t;
}

token_t lex_number() {
    int base = 10;
    int start_range1 = '0';
    int end_range1   = '9';
    int start_range2 = 'a';
    int end_range2   = 'f';

    if (code[pos] == '0') {
        if (pos+1 <= (int)code_len && code[pos+1] == 'x') {
            // base 16
            advance();
            advance();
            base = 16;
        } else if (pos+1 <= (int)code_len && code[pos+1] == 'b') {
            // base 2
            advance();
            advance();
            base = 2;
            start_range1 = '0';
            end_range1   = '1';
            start_range2 = start_range1;
            end_range2 = end_range1;
        } else {
            // base 10
            start_range2 = start_range1;
            end_range2 = end_range1;
        }
    } else {
        // base 10
        start_range2 = start_range1;
        end_range2 = end_range1;
    }
    int number = 0;
    for (int s = 0;current_char != '\0' && ((current_char >= start_range1 && current_char <= end_range1) || (current_char >= start_range2 && current_char <= end_range2)); advance(), s++) {
        if (base == 16) number = (number * base) + (current_char - start_range2 + 10);
        //else if (base == 2)
        else number = (number * base) + (current_char - start_range2);
    }

    if (number > MAX_INTEGER16_SIZE) {
        char str[31];
        sprintf(str, "Integer to big. Is %d. Max is %d", number, MAX_INTEGER16_SIZE);
        throw_error(str, true);
    }
    if (number <= MAX_INTEGER8_SIZE) {
        token_t ret_tok = {.imm16_data = number, .type = NUMBER8};
        return ret_tok;
    } else {
        token_t ret_tok = {.imm16_data = number, .type = NUMBER16};
        return ret_tok;
    }
}


// realloc tokens and append
token_t* append_token(token_t* a, int* b, token_t c) {
    *b = *b + 1;
    a = realloc(a, *b * sizeof(token_t));
    a[*b - 1] = c;
    return a;
}

token_t* lex(char* _code, size_t _code_len, int* number_of_tokens_produced) {
    code = malloc((strlen(_code)+1)*sizeof(char));
    *number_of_tokens_produced = 0;
    strcpy(code, _code);
    code_len = _code_len;
    token_t* tokens = malloc(sizeof(token_t));
    advance();
    

    for (; current_char != '\0'; advance()) {
        switch (current_char) {
            case ' ':  break;
            case '\n': break;
            case '\t': break;

            case ':':
            {
                token_t to_append = {.type = COLON, .string_data = ":"};
                tokens = append_token(tokens, number_of_tokens_produced, to_append);
            } break;
            case ',':
            {
                token_t to_append = {.type = COMMA, .string_data = ","};
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
                if (in_bounds(current_char, 'a', 'z') || in_bounds(current_char, 'A', 'Z') || current_char == '_') {
                    // ID
                    token_t token_to_append = lex_id();
                    tokens = append_token(tokens, number_of_tokens_produced, token_to_append);

                    pos--;
                } else if (in_bounds(current_char, '0', '9'))  {
                    // Number
                    token_t token_to_append = lex_number();
                    tokens = append_token(tokens, number_of_tokens_produced, token_to_append);
                    pos--;
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
