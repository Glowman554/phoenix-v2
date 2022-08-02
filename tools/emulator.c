#include "cpu.h"
#include "cpu_disasm.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define FG_ZERO (1 << 0)
#define FG_EQ (1 << 2)
#define FG_OV (1 << 3)
#define FG_HALT (1 << 4)

#define debugf(...) printf(__VA_ARGS__)

typedef struct cpu_state {
	uint16_t pc;
	uint16_t fg;
	uint8_t regs[4];
} cpu_state_t;

uint8_t cpu_fetch_byte(uint16_t addr);
void cpu_write_byte(uint16_t addr, uint8_t val);

uint8_t cpu_io_read(uint16_t addr);
void cpu_io_write(uint16_t addr, uint8_t val);

instruction_t cpu_fetch_instruction(uint16_t addr) {
	instruction_t instruction = { 0 };
	for (int i = 0; i < sizeof(instruction_t); i++) {
		((uint8_t*)&instruction)[i] = cpu_fetch_byte(addr + i);
	}

	return instruction;
}

bool cpu_tick(cpu_state_t* state) {
	instruction_t instruction = cpu_fetch_instruction(state->pc);

	// debugf("0x%x: 0x%x 0x%x 0x%x 0x%x\n", state->pc, instruction.opcode, instruction.reg1, instruction.reg2, instruction.imm);

	char disassmebled_instr[32] = { 0 };
	cpu_disasm(instruction, disassmebled_instr);
	debugf("0x%x: %s\n", state->pc, disassmebled_instr);

	uint16_t AR = (state->regs[1] << 8) | state->regs[0];
	uint16_t BR = (state->regs[3] << 8) | state->regs[2];

	switch (instruction.opcode)
	{
	case INSTR_NOP:
		break;
	case INSTR_MOV:
		state->regs[instruction.reg1] = state->regs[instruction.reg2];
		break;
	case INSTR_LOD:
		state->regs[instruction.reg1] = instruction.imm;
		break;
	case INSTR_OUT:
		cpu_io_write(AR, state->regs[instruction.reg1]);
		break;
	case INSTR_INP:
		state->regs[instruction.reg1] = cpu_io_read(AR);
		break;
	case INSTR_JNZA:
		if ((state->fg & FG_ZERO) == 0) {
			debugf("exec jmp\n");
			state->pc = AR;
			goto out;
		}
		break;
	case INSTR_ADD:
		state->regs[instruction.reg1] += state->regs[instruction.reg2];
		break;
	case INSTR_ADDI:
		state->regs[instruction.reg1] += instruction.imm;
		break;
	case INSTR_SUB:
		state->regs[instruction.reg1] -= state->regs[instruction.reg2];
		break;
	case INSTR_SUBI:
		state->regs[instruction.reg1] -= instruction.imm;
		break;
	case INSTR_NAD:
		state->regs[instruction.reg1] = ~(state->regs[instruction.reg1] & state->regs[instruction.reg2]);
		break;
	case INSTR_NADI:
		state->regs[instruction.reg1] = ~(state->regs[instruction.reg1] & instruction.imm);
		break;
	case INSTR_NOR:
		state->regs[instruction.reg1] = ~(state->regs[instruction.reg1] | state->regs[instruction.reg2]);
		break;
	case INSTR_NORI:
		state->regs[instruction.reg1] = ~(state->regs[instruction.reg1] | instruction.imm);
		break;
	case INSTR_CMP:
		state->fg = 0;

		if (state->regs[instruction.reg1] == state->regs[instruction.reg2]) {
			state->fg |= FG_EQ;
			debugf("setting bit FG_EQ\n");
		}

		if (state->regs[instruction.reg1] == 0) {
			state->fg |= FG_ZERO;
			debugf("setting bit FG_ZERO\n");
		}
		break;
	case INSTR_CMPI:
		state->fg = 0;

		if (state->regs[instruction.reg1] == instruction.imm) {
			state->fg |= FG_EQ;
			debugf("setting bit FG_EQ\n");
		}

		if (state->regs[instruction.reg1] == 0) {
			state->fg |= FG_ZERO;
			debugf("setting bit FG_ZERO\n");
		}
		break;
	case INSTR_JZRA:
		if ((state->fg & FG_ZERO) != 0) {
			debugf("exec jmp\n");
			state->pc = AR;
			goto out;
		}
		break;
	case INSTR_LDRA:
		state->regs[instruction.reg1] = cpu_fetch_byte(AR);
		break;
	case INSTR_WTRA:
		cpu_write_byte(AR, state->regs[instruction.reg1]);
		break;
	case INSTR_JMPA:
		state->pc = AR;
		goto out;
		break;
	case INSTR_JEQA:
		if ((state->fg & FG_EQ) != 0) {
			debugf("exec jmp\n");
			state->pc = AR;
			goto out;
		}
		break;
	case INSTR_JNQA:
		if ((state->fg & FG_EQ) == 0) {
			debugf("exec jmp\n");
			state->pc = AR;
			goto out;
		}
		break;
	case INSTR_JNZB:
		if ((state->fg & FG_ZERO) == 0) {
			debugf("exec jmp\n");
			state->pc = BR;
			goto out;
		}
		break;
	case INSTR_JZRB:
		if ((state->fg & FG_ZERO) != 0) {
			debugf("exec jmp\n");
			state->pc = BR;
			goto out;
		}
		break;
	case INSTR_JMPB:
		state->pc = BR;
		goto out;
		break;
	case INSTR_JEQB:
		if ((state->fg & FG_EQ) != 0) {
			debugf("exec jmp\n");
			state->pc = BR;
			goto out;
		}
		break;
	case INSTR_JNQB:
		if ((state->fg & FG_EQ) == 0) {
			debugf("exec jmp\n");
			state->pc = BR;
			goto out;
		}
		break;
	case INSTR_LDRB:
		state->regs[instruction.reg1] = cpu_fetch_byte(BR);
		break;
	case INSTR_WTRB:
		cpu_write_byte(BR, state->regs[instruction.reg1]);
		break;
	
	case INSTR_JMPI:
		state->pc = instruction.imm16;
		goto out;
		break;
	case INSTR_JEQI:
		if ((state->fg & FG_EQ) != 0) {
			debugf("exec jmp\n");
			state->pc = instruction.imm16;
			goto out;
		}
		break;
	case INSTR_JNQI:
		if ((state->fg & FG_EQ) == 0) {
			debugf("exec jmp\n");
			state->pc = instruction.imm16;
			goto out;
		}
		break;
	case INSTR_JZRI:
		if ((state->fg & FG_ZERO) != 0) {
			debugf("exec jmp\n");
			state->pc = instruction.imm16;
			goto out;
		}
		break;
	case INSTR_JNZI:
		if ((state->fg & FG_ZERO) == 0) {
			debugf("exec jmp\n");
			state->pc = instruction.imm16;
			goto out;
		}
		break;
	default:
		debugf("unk instr setting halt flag\n");
		state->fg |= FG_HALT;
		break;
	}

	state->pc += sizeof(instruction_t);

out:
	if (state->pc == 0xffff) {
		debugf("pc == 0xffff setting halt flag\n");
		state->fg |= FG_HALT;
	}

	return !((state->fg & FG_HALT) != 0);
}

void cpu_dbg(cpu_state_t* state, char* out) {
	sprintf(out, "---- CPU STATE ----\nPC: 0x%x\nFG: %s%s%s%s\nR0: 0x%x, R1: 0x%x, R2: 0x%x, R3: 0x%x\n------------------\n\n", state->pc, (state->fg & FG_ZERO) != 0 ? "FG_ZERO" : "", (state->fg & FG_EQ) != 0 ? "FG_EQ" : "", (state->fg & FG_OV) != 0 ? "FG_OV" : "", (state->fg & FG_HALT) != 0 ? "FG_HALT" : "", state->regs[0], state->regs[1], state->regs[2], state->regs[3]);
}

// -------------------------------------------------------------------------

uint8_t memory_[0xffff] = { 0 };

uint8_t cpu_fetch_byte(uint16_t addr) {
	debugf("fetching byte at 0x%x\n", addr);
	return memory_[addr];
}

void cpu_write_byte(uint16_t addr, uint8_t val) {
	debugf("writing byte 0x%x at 0x%x\n", val, addr);
	memory_[addr] = val;
}

uint8_t cpu_io_read(uint16_t addr) {
	debugf("reading byte from io at 0x%x\n", addr);
	return 0x0;
}

void cpu_io_write(uint16_t addr, uint8_t val) {
	debugf("writing byte 0x%x to io at 0x%x\n", val, addr);
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("usage: %s <file>\n", argv[0]);
		return 1;
	}

	FILE* f = fopen(argv[1], "rb");
	if (f == NULL) {
		printf("could not open file\n");
		return 1;
	}

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (size > sizeof(memory_)) {
		printf("file too large\n");
		return 1;
	}

	fread(memory_, 1, size, f);
	fclose(f);

	cpu_state_t state = { 0 };
	while (cpu_tick(&state)) {
		char out[0xff] = { 0 };
		cpu_dbg(&state, out);
		debugf("%s", out);
	}
}