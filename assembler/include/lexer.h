#pragma once

#include <stdint.h>
#include "list.h"

struct lexer_token_t
{
	int type;
	union
	{
		char data_s[0xff];
		uint8_t data_i;
	};
};

enum lexer_token_type
{
	ID,
	COLLON,
	COMMA,
	NUMBER,
	LPAREN,
	RPAREN
};

class lexer
{
	public:
		lexer(char* code);

		bool lex();

		list<lexer_token_t>* get_tokens();

	private:
		list<lexer_token_t> tokens;

		char* code;
		int code_len;

		int pos;
		char current_char;

		void advance();
		void __id();
		bool __nm();

		bool __parse_nm(char* input, int* out);
};
