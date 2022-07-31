#include "generator.h"
#include "debug.h"

void print_error(int pos_start, int pos_end, char* error);

instruction_builder_t instruction_builder(char* name, int op1, int op2, instruction_t(*build) (int ir0, int ir1, int iimm, int sreg)) {
	instruction_builder_t instr = { 0 };
	strcpy(instr.name, name);
	instr.operands[0] = op1;
	instr.operands[1] = op2;
	instr.build = build;

	// debugf("instr: %s (%d, %d)\n", name, op1, op2);

	return instr;
}


regsiter_name_list_t register_name(char* name, int id, bool sreg) {
	regsiter_name_list_t reg = { 0 };
	strcpy(reg.name, name);
	reg.id = id;
	reg.sreg = sreg;

	// debugf("reg: %s (%d, %s)\n", name, id, sreg ? "true" : "false");

	return reg;
}

extern "C" {
	instruction_t build_nop_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_mov_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_lod_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_out_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_inp_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_jnz_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_add_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_addi_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_sub_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_subi_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_nad_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_nadi_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_nor_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_nori_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_cmp_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_cmpi_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_jzr_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_ldr_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_wtr_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_jmp_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_jeq_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_jnq_instr(int ir0, int ir1, int iimm, int sreg);
}

generator::generator(list<lexer_token_t>* tokens) : register_names(10), instruction_builders(10) {
	this->tokens = tokens;
	this->pos = -1;
	this->advance();

	register_names.add(register_name((char*)"r0", R0, false));
	register_names.add(register_name((char*)"r1", R1, false));
	register_names.add(register_name((char*)"r2", R2, false));
	register_names.add(register_name((char*)"r3", R3, false));
	register_names.add(register_name((char*) "A", A, true));
	register_names.add(register_name((char*) "B", B, true));

	instruction_builders.add(instruction_builder((char*)"nop", NONE, NONE, build_nop_instr));
	instruction_builders.add(instruction_builder((char*)"mov", IR0, IR1, build_mov_instr));
	instruction_builders.add(instruction_builder((char*)"lod", IR0, IIMM, build_lod_instr));
	instruction_builders.add(instruction_builder((char*)"out", SREG, IR0, build_out_instr));
	instruction_builders.add(instruction_builder((char*)"inp", IR0, SREG, build_inp_instr));
	instruction_builders.add(instruction_builder((char*)"jnz", SREG, NONE, build_jnz_instr));
	instruction_builders.add(instruction_builder((char*)"add", IR0, IR1, build_add_instr));
	instruction_builders.add(instruction_builder((char*)"addi", IR0, IIMM, build_addi_instr));
	instruction_builders.add(instruction_builder((char*)"sub", IR0, IR1, build_sub_instr));
	instruction_builders.add(instruction_builder((char*)"subi", IR0, IIMM, build_subi_instr));
	instruction_builders.add(instruction_builder((char*)"nad", IR0, IR1, build_nad_instr));
	instruction_builders.add(instruction_builder((char*)"nadi", IR0, IIMM, build_nadi_instr));
	instruction_builders.add(instruction_builder((char*)"nor", IR0, IR1, build_nor_instr));
	instruction_builders.add(instruction_builder((char*)"nori", IR0, IIMM, build_nori_instr));
	instruction_builders.add(instruction_builder((char*)"cmp", IR0, IR1, build_cmp_instr));
	instruction_builders.add(instruction_builder((char*)"cmpi", IR0, IIMM, build_cmpi_instr));
	instruction_builders.add(instruction_builder((char*)"jzr", SREG, NONE, build_jzr_instr));
	instruction_builders.add(instruction_builder((char*)"ldr", IR0, SREG, build_ldr_instr));
	instruction_builders.add(instruction_builder((char*)"wtr", SREG, IR0, build_wtr_instr));
	instruction_builders.add(instruction_builder((char*)"jmp", SREG, NONE, build_jmp_instr));
	instruction_builders.add(instruction_builder((char*)"jeq", SREG, NONE, build_jeq_instr));
	instruction_builders.add(instruction_builder((char*)"jnq", SREG, NONE, build_jnq_instr));
}


generator::~generator() {
	if (this->data) {
		free(data);
	}
}


void generator::advance() {
	this->pos++;
	if (this->pos < this->tokens->length) {
		this->current_token = &this->tokens->data[this->pos];
	}
	else {
		this->current_token = nullptr;
		// debugf("could not load token at %d\n", this->pos);
	}
}


void generator::emit(instruction_t instr) {
	uint8_t* instr_p = (uint8_t*)&instr;

	// debugf("Emitting instruction: 0x%x 0x%x 0x%x 0x%x\n", instr.opcode, instr.reg1, instr.reg2, instr.imm);

	for (int i = 0; i < sizeof(instruction_t); i++) {
		this->data[this->current_data_pos + i] = instr_p[i];
	}

	this->current_data_pos += sizeof(instruction_t);
}


bool generator::gen() {
	uint16_t address = 0;
	list<label_t> labels = list<label_t>(10);

	debugf("Doing pass 1...\n");

	while (this->current_token) {
		if (this->current_token->data.type != ID) {
			print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected ID");
			return true;
		}

		lexer_token_t tmp = this->current_token->data;
		this->advance();

		if (this->current_token->data.type == COLLON) {
			// debugf("label %s at 0x%x\n", tmp.data_s, address);

			label_t label = { 0 };
			strcpy(label.name, tmp.data_s);
			label.address = address;
			labels.add(label);

			this->advance();
		}
		else {
			list<instruction_builder_t>::node* n = this->instruction_builders.find<char*>([](char* instr, list<instruction_builder_t>::node* n) {
				return strcmp(n->data.name, instr) == 0;
			}, tmp.data_s);

			if (n == nullptr) {
				char buf[256] = { 0 };
				sprintf(buf, "Instruction %s not found", tmp.data_s);
				print_error(tmp.pos_start, tmp.pos_end, buf);
				return true;
			}

			for (int i = 0; i < 2; i++) {
				switch (n->data.operands[i]) {
					case NONE:
						continue;
						break;
					case IR0:
					case IR1:
						if (this->current_token->data.type != ID) {
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected ID");
							return true;
						}
						break;
					case IIMM:
						if (this->current_token->data.type != ID && this->current_token->data.type != NUMBER) {
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected ID or NUMBER");
							return true;
						}
						if (this->current_token->data.type == ID) {
							if (strcmp(this->current_token->data.data_s, "lo") == 0 || strcmp(this->current_token->data.data_s, "hi") == 0) {
								this->advance();
								if (this->current_token->data.type != LPAREN) {
									print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected '('");
									return true;
								}
								this->advance();
								if (this->current_token->data.type != ID && this->current_token->data.type != NUMBER) {
									print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected ID or NUMBER");
									return true;
								}
								this->advance();
								if (this->current_token->data.type != RPAREN) {
									print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected ')'");
									return true;
								}
							}
							else {
								print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected 'lo' or 'hi'");
								return true;
							}
						}
						break;
					case SREG:
						if (this->current_token->data.type != ID) {
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected ID");
							return true;
						}
						break;
				}

				if (i == 0 && n->data.operands[1] != NONE) {
					this->advance();
					if (this->current_token->data.type != COMMA) {
						print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected ','");
						return true;
					}
				}
				this->advance();
			}

			address += sizeof(instruction_t);
		}
	}

	this->pos = -1;
	this->advance();

	data = (uint8_t*) malloc(address);
	data_len = address;
	current_data_pos = 0;

	debugf("Doing pass 2...\n");

	while (this->current_token) {
		if (this->current_token->data.type != ID) {
			print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected ID");
			return true;
		}

		lexer_token_t tmp = this->current_token->data;
		this->advance();

		if (this->current_token->data.type == COLLON) {
			this->advance();
		}
		else {
			list<instruction_builder_t>::node* n = this->instruction_builders.find<char*>([](char* instr, list<instruction_builder_t>::node* n) {
				return strcmp(n->data.name, instr) == 0;
			}, tmp.data_s);

			if (n == nullptr) {
				char buf[256] = { 0 };
				sprintf(buf, "Instruction %s not found", tmp.data_s);
				print_error(tmp.pos_start, tmp.pos_end, buf);
				return true;
			}

			int ir0 = 0;
			int ir1 = 0;
			int iimm = 0;
			int sreg = 0;

			for (int i = 0; i < 2; i++) {
				switch (n->data.operands[i]) {
					case NONE:
						continue;
						break;

					case IR0:
					{
						if (this->current_token->data.type != ID) {
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected ID");
							return true;
						}

						list<regsiter_name_list_t>::node* r = this->register_names.find<char*>([](char* name, list<regsiter_name_list_t>::node* n) {
							return strcmp(name, n->data.name) == 0;
						}, this->current_token->data.data_s);

						if (r == nullptr) {
							char buf[256] = { 0 };
							sprintf(buf, "Unknown register: %s", this->current_token->data.data_s);
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, buf);
							return true;
						}

						if (r->data.sreg) {
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected IR0 and not SREG");
							return true;
						}

						ir0 = r->data.id;
					}
					break;

					case IR1:
					{
						if (this->current_token->data.type != ID) {
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected ID");
							return true;
						}

						list<regsiter_name_list_t>::node* r = this->register_names.find<char*>([](char* name, list<regsiter_name_list_t>::node* n) {
							return strcmp(name, n->data.name) == 0;
						}, this->current_token->data.data_s);

						if (r == nullptr) {
							char buf[256] = { 0 };
							sprintf(buf, "Unknown register: %s", this->current_token->data.data_s);
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, buf);
							return true;
						}

						if (r->data.sreg) {
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected IR1 and not SREG");
							return true;
						}

						ir1 = r->data.id;
					}
					break;

					case IIMM:
						if (this->current_token->data.type != ID && this->current_token->data.type != NUMBER) {
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected ID or NUMBER");
							return true;
						}
						if (this->current_token->data.type != ID && this->current_token->data.type != NUMBER) {
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected ID or NUMBER");
							return true;
						}
						if (this->current_token->data.type == ID) {
							bool lo_or_hi = false;
							if (strcmp(this->current_token->data.data_s, "lo") == 0 || strcmp(this->current_token->data.data_s, "hi") == 0) {
								if (strcmp(this->current_token->data.data_s, "lo") == 0) {
									lo_or_hi = false;
								}
								else {
									lo_or_hi = true;
								}

								this->advance();
								if (this->current_token->data.type != LPAREN) {
									print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected LPAREN");
									return true;
								}
								this->advance();
								if (this->current_token->data.type != ID && this->current_token->data.type != NUMBER) {
									print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected ID or NUMBER");
									return true;
								}

								uint16_t num = 0;

								if (this->current_token->data.type == NUMBER) {
									num = this->current_token->data.data_i;
								}
								else {
									list<label_t>::node* l = labels.find<char*>([](char* name, list<label_t>::node* n) {
										return strcmp(name, n->data.name) == 0;
									}, this->current_token->data.data_s);

									if (l == nullptr) {
										char buf[256];
										sprintf(buf, "Label %s not found", this->current_token->data.data_s);
										print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, buf);
										return true;
									}

									num = l->data.address;
								}

								if (lo_or_hi) {
									iimm = (num & 0xff00) >> 8;
								}
								else {
									iimm = num & 0x00ff;
								}

								this->advance();
								if (this->current_token->data.type != RPAREN) {
									print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected RPAREN");
									return true;
								}
							}
							else {
								print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected 'lo' or 'hi'");
								return true;
							}
						}
						else {
							iimm = this->current_token->data.data_i;
						}
						break;

					case SREG:
					{
						if (this->current_token->data.type != ID) {
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected ID");
							return true;
						}

						list<regsiter_name_list_t>::node* r = this->register_names.find<char*>([](char* name, list<regsiter_name_list_t>::node* n) {
							return strcmp(name, n->data.name) == 0;
						}, this->current_token->data.data_s);

						if (r == nullptr) {
							char buf[256] = { 0 };
							sprintf(buf, "Unknown register: %s", this->current_token->data.data_s);
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, buf);
							return true;
						}

						if (!r->data.sreg) {
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected SREG");
							return true;
						}

						sreg = r->data.id;
					}
					break;
				}

				if (i == 0 && n->data.operands[1] != NONE) {
					this->advance();
					if (this->current_token->data.type != COMMA) {
						print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, "Expected COMMA");
						return true;
					}
				}
				this->advance();
			}

			instruction_t i = n->data.build(ir0, ir1, iimm, sreg);
			emit(i);
		}
	}

	return false;
}


uint8_t* generator::get_data() {
	return this->data;
}


uint16_t generator::get_data_len() {
	return this->data_len;
}
