#include "lexer.h"
#include "debug.h"

#include <string.h>

#ifdef _MSC_VER
#pragma warning(disable : 4996)	 //_CRT_SECURE_NO_WARNINGS
#endif

void print_error(int pos_start, int pos_end, char* error);

lexer::lexer(char* code) : tokens(10) {
	this->code = code;
	this->code_len = strlen(code);

	this->pos = -1;
	this->advance();
}


list<lexer_token_t>* lexer::get_tokens() {
	return &this->tokens;
}


void lexer::advance() {
	this->pos++;

	if (this->pos < this->code_len) {
		this->current_char = this->code[this->pos];
		// debugf("loaded char %c at %d\n", this->current_char, this->pos);
	}
	else {
		this->current_char = '\0';
		// debugf("could not load char at %d\n", this->pos);
	}
}


void lexer::__id() {
	int pos_start = this->pos;
	char buf[0xff] = { 0 };
	int idx = 0;
	do {
		buf[idx++] = this->current_char;
		this->advance();
	} while ((this->current_char >= 'a' && this->current_char <= 'z') || (this->current_char >= 'A' && this->current_char <= 'Z') || (this->current_char >= '0' && this->current_char <= '9'));
	// debugf("buf: %s\n", buf);

	lexer_token_t token = { 0 };
	token.type = ID;
	token.pos_start = pos_start;
	token.pos_end = this->pos;
	strcpy(token.data_s, buf);
	this->tokens.add(token);
}


bool lexer::__parse_nm(char* input, int* out) {
	int idx = 0;
	int number_system_base = 10;
	
    if (input[0] == '0') {
		if (input[1] == 'x') {
			number_system_base = 16;
			idx = 2;
		}
		else if (input[1] == 'b') {
            number_system_base = 2;
			idx = 2;
		}
	}


	int _number = 0;

	while (input[idx] != '\0') {
		if (input[idx] >= '0' && input[idx] <= '9') {
			_number = _number * number_system_base + (input[idx] - '0');
		}
		else if (input[idx] >= 'a' && input[idx] <= 'f') {
			_number = _number * number_system_base + (input[idx] - 'a' + 10);
		}
		else if (input[idx] >= 'A' && input[idx] <= 'F') {
			_number = _number * number_system_base + (input[idx] - 'A' + 10);
		}
		else {
			char buf[0xff] = { 0 };
			sprintf(buf, "Unexpected char %c in number", input[idx]);
			print_error(this->pos, this->pos + 1, buf);
			return true;
		}

		idx++;
	}

	*out = _number;
	return false;
}


bool lexer::__nm() {
	int pos_start = this->pos;
	char buf[0xff] = { 0 };
	int idx = 0;
	do {
		buf[idx++] = this->current_char;
		this->advance();
	} while ((this->current_char >= '0' && this->current_char <= '9') || this->current_char == 'x' || this->current_char == 'b' || (this->current_char >= 'a' && this->current_char <= 'f') || (this->current_char >= 'A' && this->current_char <= 'F'));

	int number = 0;
	if (this->__parse_nm(buf, &number)) {
		return true;
	}

	// debugf("number: %d\n", number);

	lexer_token_t token = { 0 };
	token.type = NUMBER;
	token.pos_start = pos_start;
	token.pos_end = this->pos;
	token.data_i = number;
	this->tokens.add(token);

	return false;
}


bool lexer::lex() {
	debugf("Lexing...\n");
	
	while (this->current_char) {

		if ((this->current_char >= 'a' && this->current_char <= 'z') || (this->current_char >= 'A' && this->current_char <= 'Z')) {
			this->__id();
			continue;
		}

		if ((this->current_char >= '0' && this->current_char <= '9')) {
			if (this->__nm()) {
				debugf("Unexpected number format\n");
				return true;
			}

			continue;
		}

		switch (this->current_char) {
			case ' ':
			case '\n':
            case '\r':
            case '\t':
				break;

			case ':':
			{
				lexer_token_t token = { 0 };
				token.type = COLLON;
				token.pos_start = this->pos;
				token.pos_end = this->pos + 1;
				this->tokens.add(token);
			}
			break;

			case ',':
			{
				lexer_token_t token = { 0 };
				token.type = COMMA;
				token.pos_start = this->pos;
				token.pos_end = this->pos + 1;
				this->tokens.add(token);
			}
			break;

			case '(':
			{
				lexer_token_t token = { 0 };
				token.type = LPAREN;
				token.pos_start = this->pos;
				token.pos_end = this->pos + 1;
				this->tokens.add(token);
			}
			break;

			case ')':
			{
				lexer_token_t token = { 0 };
				token.type = RPAREN;
				token.pos_start = this->pos;
				token.pos_end = this->pos + 1;
				this->tokens.add(token);
			}
			break;

			case ';':
			{
				while (this->current_char != '\n' && this->current_char != '\0') {
					this->advance();
				}
			}
			break;

			default:
				print_error(this->pos, this->pos + 1, (char*)"Unexpected token");
				return true;
		}

		this->advance();

	}

	return false;
}
