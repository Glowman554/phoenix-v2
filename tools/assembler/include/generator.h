#pragma once

#include "lexer.h"
#include "cpu.h"

enum generator_instr_operands
{
	NONE,
	IR0,
	IR1,
	IIMM,
	SREG						 // Super register (A)
};

struct instruction_builder_t
{
	instruction_t(*build) (int ir0, int ir1, int iimm, int sreg);
	char name[5];

	int operands[2];
};

struct regsiter_name_list_t
{
	char name[3];
	int id;
	bool sreg;
};

struct label_t
{
	uint16_t address;
	char name[0xff];
};

class generator
{
	public:
		generator(list<lexer_token_t>* tokens);
		~generator();

		bool gen();

		uint8_t* get_data();
		uint16_t get_data_len();

	private:
		list<lexer_token_t>* tokens;
		int pos;
		list<lexer_token_t>::node* current_token;

		uint8_t* data;
		uint16_t data_len;
		uint16_t current_data_pos;

		void advance();

		void emit(instruction_t instr);

		list<regsiter_name_list_t> register_names;
		list <instruction_builder_t> instruction_builders;
};
