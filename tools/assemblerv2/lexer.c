#include <errors.h>
#include <lexer.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>

int pos = -1;
size_t code_len = 0;
char* code;
char current_char = '\0';

void advance() {
	if (pos > (int)code_len)
		current_char = '\0';
	else {
		pos++;
		current_char = code[pos];
	}
}

token_t lex_id() {
	token_t t = {.type = ID};
	int start_pos = pos;
	while (current_char != '\0' && (in_bounds(current_char, 'a', 'z') || in_bounds(current_char, 'A', 'Z') || in_bounds(current_char, '0', '9') || current_char == '_')) {
		if ((pos - start_pos) >= (MAX_LABEL_SIZE - 1)) {
			// error. Label to long
			throw_error("Label to long!", true);
		}
		t.string_data[pos - start_pos] = current_char;
		advance();
	}
	t.string_data[pos - start_pos] = '\0';
	return t;
}

token_t lex_number() {
	int idx = 0;
	int number_system_base = 10;
	
    if (current_char == '0') {
		advance();
		if (current_char == 'x') {
			number_system_base = 16;
			idx = 2;
		}
		else if (current_char == 'b') {
            number_system_base = 2;
			idx = 2;
		}
		advance();
	}


	int _number = 0;

	while (current_char != '\0' && (in_bounds(current_char, '0', '9') || in_bounds(current_char, 'a', 'f') || in_bounds(current_char, 'A', 'F'))) {
		if (current_char >= '0' && current_char <= '9') {
			_number = _number * number_system_base + (current_char - '0');
		}
		else if (current_char >= 'a' && current_char <= 'f') {
			_number = _number * number_system_base + (current_char - 'a' + 10);
		}
		else if (current_char >= 'A' && current_char <= 'F') {
			_number = _number * number_system_base + (current_char - 'A' + 10);
		}
		else {
			char buf[0xff] = { 0 };
			sprintf(buf, "Unexpected char %c in number", current_char);
			throw_error(buf, true);
			token_t t;
			return t;
		}

		advance();
	}

	if (_number > MAX_INTEGER16_SIZE) {
		char str[0xff];
		sprintf(str, "Integer to big. Is %d. Max is %d", _number, MAX_INTEGER16_SIZE);
		throw_error(str, true);
	}
	if (_number <= MAX_INTEGER8_SIZE) {
		token_t ret_tok = {.imm16_data = _number, .type = NUMBER8};
		return ret_tok;
	} else {
		token_t ret_tok = {.imm16_data = _number, .type = NUMBER16};
		return ret_tok;
	}
}

void lex_comment() {
    while (code[pos] != '\n' && code[pos] != '\0') {
		advance();
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
	code = malloc((strlen(_code) + 1) * sizeof(char));
	*number_of_tokens_produced = 0;
	strcpy(code, _code);
	code_len = _code_len;
	token_t* tokens = malloc(sizeof(token_t));
	advance();

	for (; current_char != '\0'; advance()) {
		switch (current_char) {
		case ' ':
			break;
		case '\n':
			break;
		case '\t':
			break;

		case ':': {
			token_t to_append = {.type = COLON, .string_data = ":"};
			tokens = append_token(tokens, number_of_tokens_produced, to_append);
		} break;
		case ',': {
			token_t to_append = {.type = COMMA, .string_data = ","};
			tokens = append_token(tokens, number_of_tokens_produced, to_append);
		} break;
		case '(': {
			token_t to_append = {.type = LPAREN, .string_data = "("};
			tokens = append_token(tokens, number_of_tokens_produced, to_append);
		} break;
		case ')': {
			token_t to_append = {.type = RPAREN, .string_data = ")"};
			tokens = append_token(tokens, number_of_tokens_produced, to_append);
		} break;
		case '$': {
			token_t to_append = {.type = DOLLAR, .string_data = "$"};
			tokens = append_token(tokens, number_of_tokens_produced, to_append);
		} break;
        case ';': {
            lex_comment();
        } break;
		

		default: {
			// check for IDs and Numbers. otherwise throw error
			if (in_bounds(current_char, 'a', 'z') || in_bounds(current_char, 'A', 'Z') || current_char == '_') {
				// ID
				token_t token_to_append = lex_id();
				tokens = append_token(tokens, number_of_tokens_produced, token_to_append);

				pos--;
			} else if (in_bounds(current_char, '0', '9')) {
				// Number
				token_t token_to_append = lex_number();
				tokens = append_token(tokens, number_of_tokens_produced, token_to_append);
				pos--;
			} else {
				char str[18];
				sprintf(str, "Unknown Token: %d", current_char);
				throw_error(str, true);
			}
		} break;
		}
	}

	return tokens;
}
