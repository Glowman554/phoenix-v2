#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

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

	instruction_t* instructions = (instruction_t*) malloc(size);

	fread(instructions, 1, size, f);
	fclose(f);

	for (int i = 0; i < size / sizeof(instruction_t); i++) {
		char out[256];
		disasm(instructions[i], out);
		printf("0x%lx:\t%s\n", i * sizeof(instruction_t), out);
	}
}