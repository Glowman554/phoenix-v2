#pragma once

#include <cpu.h>
#include <stdio.h>

static inline void cpu_disasm(instruction_t instruction, char* out) {
	char* regs[] = { "r0", "r1", "r2", "r3", "r4", "r5", "unk", "unk", "unk", "unk", "unk", "unk", "unk", "unk", "unk", "unk" };

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
	case INSTR_JNZA:
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
	case INSTR_JZRA:
		sprintf(out, "jzr A");
		break;
	case INSTR_LDRA:
		sprintf(out, "ldr %s, A", regs[instruction.reg1]);
		break;
	case INSTR_WTRA:
		sprintf(out, "wtr A, %s", regs[instruction.reg1]);
		break;
	case INSTR_JMPA:
		sprintf(out, "jmp A");
		break;
	case INSTR_JEQA:
		sprintf(out, "jeq A");
		break;
	case INSTR_JNQA:
		sprintf(out, "jnq A");
		break;
	case INSTR_JNZB:
		sprintf(out, "jnz B");
		break;
	case INSTR_JZRB:
		sprintf(out, "jzr B");
		break;
	case INSTR_JMPB:
		sprintf(out, "jmp B");
		break;
	case INSTR_JEQB:
		sprintf(out, "jeq B");
		break;
	case INSTR_JNQB:
		sprintf(out, "jnq B");
		break;
	case INSTR_LDRB:
		sprintf(out, "ldr %s, B", regs[instruction.reg1]);
		break;
	case INSTR_WTRB:
		sprintf(out, "wtr B, %s", regs[instruction.reg1]);
		break;
	case INSTR_JMPI:
		sprintf(out, "jmp 0x%x", instruction.imm16);
		break;
	case INSTR_JEQI:
		sprintf(out, "jeq 0x%x", instruction.imm16);
		break;
	case INSTR_JNQI:
		sprintf(out, "jnq 0x%x", instruction.imm16);
		break;
	case INSTR_JZRI:
		sprintf(out, "jzr 0x%x", instruction.imm16);
		break;
	case INSTR_JNZI:
		sprintf(out, "jnz 0x%x", instruction.imm16);
		break;
	default:
		sprintf(out, "unk 0x%x", instruction.opcode);
		break;
	}
}
