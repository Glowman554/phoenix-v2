#include "cpu.h"
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
	uint8_t regs[3];
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

void disasm(instruction_t instruction, char* out) {
	char* regs[] = { "r0", "r1", "r2" };

	switch (instruction.opcode)
	{
	case INSTR_NOP:
		sprintf(out, "nop");
		break;
	case INSTR_MOV:
		sprintf(out, "mov %s, %s", regs[instruction.reg1], regs[instruction.reg2]);
		break;
	case INSTR_LOD:
		sprintf(out, "lod %s, 0x%x", regs[instruction.reg1], instruction.imm);
		break;
	case INSTR_OUT:
		sprintf(out, "out A, %s", regs[instruction.reg1]);
		break;
	case INSTR_INP:
		sprintf(out, "inp %s, A", regs[instruction.reg1]);
		break;
	case INSTR_JNZ:
		sprintf(out, "jnz A");
		break;
	case INSTR_ADD:
		sprintf(out, "add %s, %s", regs[instruction.reg1], regs[instruction.reg2]);
		break;
	case INSTR_ADDI:
		sprintf(out, "add %s, 0x%x", regs[instruction.reg1], instruction.imm);
		break;
	case INSTR_SUB:
		sprintf(out, "sub %s, %s", regs[instruction.reg1], regs[instruction.reg2]);
		break;
	case INSTR_SUBI:
		sprintf(out, "sub %s, 0x%x", regs[instruction.reg1], instruction.imm);
		break;
	case INSTR_NAD:
		sprintf(out, "nad %s, %s", regs[instruction.reg1], regs[instruction.reg2]);
		break;
	case INSTR_NADI:
		sprintf(out, "nad %s, 0x%x", regs[instruction.reg1], instruction.imm);
		break;
	case INSTR_NOR:
		sprintf(out, "nor %s, %s", regs[instruction.reg1], regs[instruction.reg2]);
		break;
	case INSTR_NORI:
		sprintf(out, "nor %s, 0x%x", regs[instruction.reg1], instruction.imm);
		break;
	case INSTR_CMP:
		sprintf(out, "cmp %s, %s", regs[instruction.reg1], regs[instruction.reg2]);
		break;
	case INSTR_CMPI:
		sprintf(out, "cmp %s, 0x%x", regs[instruction.reg1], instruction.imm);
		break;
	case INSTR_JZR:
		sprintf(out, "jzr A");
		break;
	case INSTR_LDR:
		sprintf(out, "ldr %s, A", regs[instruction.reg1]);
		break;
	case INSTR_WTR:
		sprintf(out, "wtr A, %s", regs[instruction.reg1]);
		break;
	case INSTR_JMP:
		sprintf(out, "jmp A");
		break;
	case INSTR_JEQ:
		sprintf(out, "jeq A");
		break;
	case INSTR_JNQ:
		sprintf(out, "jnq A");
		break;
	default:
		sprintf(out, "unk 0x%x", instruction.opcode);
		break;
	}
}

bool cpu_tick(cpu_state_t* state) {
	instruction_t instruction = cpu_fetch_instruction(state->pc);

	// debugf("0x%x: 0x%x 0x%x 0x%x 0x%x\n", state->pc, instruction.opcode, instruction.reg1, instruction.reg2, instruction.imm);

	char disassmebled_instr[32] = { 0 };
	disasm(instruction, disassmebled_instr);
	debugf("0x%x: %s\n", state->pc, disassmebled_instr);

	uint16_t AR = (state->regs[1] << 8) | state->regs[0];;

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
	case INSTR_JNZ:
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
	case INSTR_JZR:
		if ((state->fg & FG_ZERO) != 0) {
			debugf("exec jmp\n");
			state->pc = AR;
			goto out;
		}
		break;
	case INSTR_LDR:
		state->regs[instruction.reg1] = cpu_fetch_byte(AR);
		break;
	case INSTR_WTR:
		cpu_write_byte(AR, state->regs[instruction.reg1]);
		break;
	case INSTR_JMP:
		state->pc = AR;
		goto out;
		break;
	case INSTR_JEQ:
		if ((state->fg & FG_EQ) != 0) {
			debugf("exec jmp\n");
			state->pc = AR;
			goto out;
		}
		break;
	case INSTR_JNQ:
		if ((state->fg & FG_EQ) == 0) {
			debugf("exec jmp\n");
			state->pc = AR;
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
	sprintf(out, "---- CPU STATE ----\nPC: 0x%x\nFG: %s%s%s%s\nR0: 0x%x, R1: 0x%x, R2: 0x%x\n------------------\n\n", state->pc, (state->fg & FG_ZERO) != 0 ? "FG_ZERO" : "", (state->fg & FG_EQ) != 0 ? "FG_EQ" : "", (state->fg & FG_OV) != 0 ? "FG_OV" : "", (state->fg & FG_HALT) != 0 ? "FG_HALT" : "", state->regs[0], state->regs[1], state->regs[2]);
}

// -------------------------------------------------------------------------

uint8_t memory[0xffff] = { 0 };

uint8_t cpu_fetch_byte(uint16_t addr) {
	debugf("fetching byte at 0x%x\n", addr);
	return memory[addr];
}

void cpu_write_byte(uint16_t addr, uint8_t val) {
	debugf("writing byte 0x%x at 0x%x\n", val, addr);
	memory[addr] = val;
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

	if (size > sizeof(memory)) {
		printf("file too large\n");
		return 1;
	}

	fread(memory, 1, size, f);
	fclose(f);

	cpu_state_t state = { 0 };
	while (cpu_tick(&state)) {
		char out[0xff] = { 0 };
		cpu_dbg(&state, out);
		debugf("%s", out);
	}
}