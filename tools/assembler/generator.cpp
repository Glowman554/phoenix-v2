#include "generator.h"
#include "debug.h"

void print_error(int pos_start, int pos_end, char* error);

instruction_builder_t instruction_builder(char* name, int op1, int op2, instruction_t(*build) (int ir0, int ir1, int iimm, int sreg)) {
	instruction_builder_t instr = { 0 };
	strcpy(instr.name, name);
	instr.operands[0] = op1;
	instr.operands[1] = op2;
	instr.build = build;

	debugf("instr: %s (%d, %d)\n", name, op1, op2);

	return instr;
}


regsiter_name_list_t register_name(char* name, int id, bool sreg) {
	regsiter_name_list_t reg = { 0 };
	strcpy(reg.name, name);
	reg.id = id;
	reg.sreg = sreg;

	debugf("reg: %s (%d, %s)\n", name, id, sreg ? "true" : "false");

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

	instruction_t build_jmpi_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_jeqi_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_jnqi_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_jzri_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_jnzi_instr(int ir0, int ir1, int iimm, int sreg);

	instruction_t build_lad_instr(int ir0, int ir1, int iimm, int sreg);

	instruction_t build_ire_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_int_instr(int ir0, int ir1, int iimm, int sreg);

	instruction_t build_jof_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_jofi_instr(int ir0, int ir1, int iimm, int sreg);

	instruction_t build_jno_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_jnoi_instr(int ir0, int ir1, int iimm, int sreg);

	instruction_t build_ado_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_adoi_instr(int ir0, int ir1, int iimm, int sreg);

	instruction_t build_sbo_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_sboi_instr(int ir0, int ir1, int iimm, int sreg);

	instruction_t build_cfg_instr(int ir0, int ir1, int iimm, int sreg);

	instruction_t build_lih_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_lihi_instr(int ir0, int ir1, int iimm, int sreg);

	instruction_t build_wfg_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_rfg_instr(int ir0, int ir1, int iimm, int sreg);

	instruction_t build_lsp_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_lspi_instr(int ir0, int ir1, int iimm, int sreg);

	instruction_t build_rsp_instr(int ir0, int ir1, int iimm, int sreg);

	instruction_t build_put_instr(int ir0, int ir1, int iimm, int sreg);
	instruction_t build_puti_instr(int ir0, int ir1, int iimm, int sreg);

	instruction_t build_pop_instr(int ir0, int ir1, int iimm, int sreg);

}

generator::generator(list<lexer_token_t>* tokens) : register_names(10), instruction_builders(10) {
	this->tokens = tokens;
	this->pos = -1;
	this->advance();

	register_names.add(register_name((char*)"r0", R0, false));
	register_names.add(register_name((char*)"r1", R1, false));
	register_names.add(register_name((char*)"r2", R2, false));
	register_names.add(register_name((char*)"r3", R3, false));
	register_names.add(register_name((char*)"r4", R4, false));
	register_names.add(register_name((char*)"r5", R5, false));
	register_names.add(register_name((char*)"r6", R6, false));
	register_names.add(register_name((char*)"r7", R7, false));
	register_names.add(register_name((char*)"r8", R8, false));
	register_names.add(register_name((char*)"r9", R9, false));
	register_names.add(register_name((char*)"r10", R10, false));
	register_names.add(register_name((char*)"r11", R11, false));
	register_names.add(register_name((char*)"r12", R12, false));
	register_names.add(register_name((char*)"r13", R13, false));
	register_names.add(register_name((char*)"r14", R14, false));
	register_names.add(register_name((char*)"r15", R15, false));

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
	instruction_builders.add(instruction_builder((char*)"jmpi", IIMM, NONE, build_jmpi_instr));
	instruction_builders.add(instruction_builder((char*)"jeqi", IIMM, NONE, build_jeqi_instr));
	instruction_builders.add(instruction_builder((char*)"jnqi", IIMM, NONE, build_jnqi_instr));
	instruction_builders.add(instruction_builder((char*)"jzri", IIMM, NONE, build_jzri_instr));
	instruction_builders.add(instruction_builder((char*)"jnzi", IIMM, NONE, build_jnzi_instr));
	instruction_builders.add(instruction_builder((char*)"lad", SREG, IIMM, build_lad_instr));
	instruction_builders.add(instruction_builder((char*)"ire", NONE, NONE, build_ire_instr));
	instruction_builders.add(instruction_builder((char*)"int", NONE, NONE, build_int_instr));

	instruction_builders.add(instruction_builder((char*)"jof", SREG, NONE, build_jof_instr));
	instruction_builders.add(instruction_builder((char*)"jofi", IIMM, NONE, build_jofi_instr));

	instruction_builders.add(instruction_builder((char*)"jno", SREG, NONE, build_jno_instr));
	instruction_builders.add(instruction_builder((char*)"jnoi", IIMM, NONE, build_jnoi_instr));

	instruction_builders.add(instruction_builder((char*)"ado",IR0, IR1, build_ado_instr));
	instruction_builders.add(instruction_builder((char*)"adoi", IR0, IIMM, build_adoi_instr));

	instruction_builders.add(instruction_builder((char*)"sbo", IR0, IR1, build_sbo_instr));
	instruction_builders.add(instruction_builder((char*)"sboi", IR0, IIMM, build_sboi_instr));

	instruction_builders.add(instruction_builder((char*)"cfg", NONE, NONE, build_cfg_instr));

	instruction_builders.add(instruction_builder((char*)"lih", SREG, NONE, build_lih_instr));
	instruction_builders.add(instruction_builder((char*)"lihi", IIMM, NONE, build_lihi_instr));

	instruction_builders.add(instruction_builder((char*)"wfg", IR0, NONE, build_wfg_instr));
	instruction_builders.add(instruction_builder((char*)"rfg", IR0, NONE, build_rfg_instr));

	instruction_builders.add(instruction_builder((char*)"lsp", SREG, NONE, build_lsp_instr));
	instruction_builders.add(instruction_builder((char*)"lspi", IIMM, NONE, build_lspi_instr));

	instruction_builders.add(instruction_builder((char*)"rsp", SREG, NONE, build_rsp_instr));

	instruction_builders.add(instruction_builder((char*)"put", IR0, NONE, build_put_instr));
	instruction_builders.add(instruction_builder((char*)"puti", IIMM, NONE, build_puti_instr));

	instruction_builders.add(instruction_builder((char*)"pop", IR0, NONE, build_pop_instr));
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
			print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected ID");
			return true;
		}

		lexer_token_t tmp = this->current_token->data;
		this->advance();

		if (this->current_token && this->current_token->data.type == COLLON) {
			debugf("label %s at 0x%x\n", tmp.data_s, address);

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
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected ID");
							return true;
						}
						break;
					case IIMM:
						if (this->current_token->data.type != ID && this->current_token->data.type != NUMBER) {
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected ID or NUMBER");
							return true;
						}
						if (this->current_token->data.type == ID) {
							if (strcmp(this->current_token->data.data_s, "lo") == 0 || strcmp(this->current_token->data.data_s, "hi") == 0 || strcmp(this->current_token->data.data_s, "addr") == 0) {
								this->advance();
								if (this->current_token->data.type != LPAREN) {
									print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected '('");
									return true;
								}
								this->advance();
								if (this->current_token->data.type != ID && this->current_token->data.type != NUMBER) {
									print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected ID or NUMBER");
									return true;
								}
								this->advance();
								if (this->current_token->data.type != RPAREN) {
									print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected ')'");
									return true;
								}
							}
							else {
								print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected 'lo' or 'hi' or 'addr'");
								return true;
							}
						}
						break;
					case SREG:
						if (this->current_token->data.type != ID) {
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected ID");
							return true;
						}
						break;
				}

				if (i == 0 && n->data.operands[1] != NONE) {
					this->advance();
					if (this->current_token->data.type != COMMA) {
						print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected ','");
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
			print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected ID");
			return true;
		}

		lexer_token_t tmp = this->current_token->data;
		this->advance();

		if (this->current_token && this->current_token->data.type == COLLON) {
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
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected ID");
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
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected IR0 and not SREG");
							return true;
						}

						ir0 = r->data.id;
					}
					break;

					case IR1:
					{
						if (this->current_token->data.type != ID) {
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected ID");
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
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected IR1 and not SREG");
							return true;
						}

						ir1 = r->data.id;
					}
					break;

					case IIMM:
						if (this->current_token->data.type != ID && this->current_token->data.type != NUMBER) {
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected ID or NUMBER");
							return true;
						}
						if (this->current_token->data.type != ID && this->current_token->data.type != NUMBER) {
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected ID or NUMBER");
							return true;
						}
						if (this->current_token->data.type == ID) {
							bool lo_or_hi = false;
							bool lo_or_hi_found = false;
							if (strcmp(this->current_token->data.data_s, "lo") == 0 || strcmp(this->current_token->data.data_s, "hi") == 0 || strcmp(this->current_token->data.data_s, "addr") == 0) {
								if (strcmp(this->current_token->data.data_s, "lo") == 0) {
									lo_or_hi = false;
									lo_or_hi_found = true;
								}
								else if (strcmp(this->current_token->data.data_s, "hi") == 0) {
									lo_or_hi = true;
									lo_or_hi_found = true;
								}

								this->advance();
								if (this->current_token->data.type != LPAREN) {
									print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected LPAREN");
									return true;
								}
								this->advance();
								if (this->current_token->data.type != ID && this->current_token->data.type != NUMBER) {
									print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected ID or NUMBER");
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

								if (lo_or_hi_found) {
									if (lo_or_hi) {
										iimm = (num & 0xff00) >> 8;
									}
									else {
										iimm = num & 0x00ff;
									}
								} else {
									// debugf("%d\n", num);
									iimm = num;
								}

								this->advance();
								if (this->current_token->data.type != RPAREN) {
									print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected RPAREN");
									return true;
								}
							}
							else {
								print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected 'lo' or 'hi' or 'addr'");
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
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected ID");
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
							print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected SREG");
							return true;
						}

						sreg = r->data.id;
					}
					break;
				}

				if (i == 0 && n->data.operands[1] != NONE) {
					this->advance();
					if (this->current_token->data.type != COMMA) {
						print_error(this->current_token->data.pos_start, this->current_token->data.pos_end, (char*)"Expected COMMA");
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
