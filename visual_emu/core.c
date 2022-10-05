#include <core.h>

#include <stddef.h>
#include <stdio.h>
#include <debug.h>
#include <cpu_disasm.h>


instruction_t cpu_fetch_instruction(uint16_t addr) {
	instruction_t instruction = { 0 };
	for (size_t i = 0; i < sizeof(instruction_t); i++) {
		((uint8_t*)&instruction)[i] = cpu_fetch_byte(addr + i);
	}

	return instruction;
}

void cpu_write_instruction(instruction_t instr, uint16_t addr) {
	for (size_t i = 0; i < sizeof(instruction_t); i++) {
		cpu_write_byte(addr + i, ((uint8_t*)&instr)[i]);
	}
}

bool cpu_tick(cpu_state_t* state) {
	instruction_t instruction = cpu_fetch_instruction(state->pc);

	// debugf("0x%x: 0x%x 0x%x 0x%x 0x%x", state->pc, instruction.opcode, instruction.reg1, instruction.reg2, instruction.imm);

	char disassmebled_instr[32] = { 0 };
	cpu_disasm(instruction, disassmebled_instr);
	debugf("0x%x: %s", state->pc, disassmebled_instr);

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
			debugf("exec jmp");
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
			debugf("setting bit FG_EQ");
		}

		if (state->regs[instruction.reg1] == 0) {
			state->fg |= FG_ZERO;
			debugf("setting bit FG_ZERO");
		}
		break;
	case INSTR_CMPI:
		state->fg = 0;

		if (state->regs[instruction.reg1] == instruction.imm) {
			state->fg |= FG_EQ;
			debugf("setting bit FG_EQ");
		}

		if (state->regs[instruction.reg1] == 0) {
			state->fg |= FG_ZERO;
			debugf("setting bit FG_ZERO");
		}
		break;
	case INSTR_JZRA:
		if ((state->fg & FG_ZERO) != 0) {
			debugf("exec jmp");
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
			debugf("exec jmp");
			state->pc = AR;
			goto out;
		}
		break;
	case INSTR_JNQA:
		if ((state->fg & FG_EQ) == 0) {
			debugf("exec jmp");
			state->pc = AR;
			goto out;
		}
		break;
	case INSTR_JNZB:
		if ((state->fg & FG_ZERO) == 0) {
			debugf("exec jmp");
			state->pc = BR;
			goto out;
		}
		break;
	case INSTR_JZRB:
		if ((state->fg & FG_ZERO) != 0) {
			debugf("exec jmp");
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
			debugf("exec jmp");
			state->pc = BR;
			goto out;
		}
		break;
	case INSTR_JNQB:
		if ((state->fg & FG_EQ) == 0) {
			debugf("exec jmp");
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
			debugf("exec jmp");
			state->pc = instruction.imm16;
			goto out;
		}
		break;
	case INSTR_JNQI:
		if ((state->fg & FG_EQ) == 0) {
			debugf("exec jmp");
			state->pc = instruction.imm16;
			goto out;
		}
		break;
	case INSTR_JZRI:
		if ((state->fg & FG_ZERO) != 0) {
			debugf("exec jmp");
			state->pc = instruction.imm16;
			goto out;
		}
		break;
	case INSTR_JNZI:
		if ((state->fg & FG_ZERO) == 0) {
			debugf("exec jmp");
			state->pc = instruction.imm16;
			goto out;
		}
		break;
	default:
		debugf("unk instr setting halt flag");
		state->fg |= FG_HALT;
		break;
	}

	state->pc += sizeof(instruction_t);

out:
	if (state->pc == 0xffff) {
		debugf("pc == 0xffff setting halt flag");
		state->fg |= FG_HALT;
	}

	return !((state->fg & FG_HALT) != 0);
}

void cpu_dbg(cpu_state_t* state, char* out) {
	sprintf(out, "---- CPU STATE ----\n\rPC: 0x%x\n\rFG: %s%s%s%s\n\rR0: 0x%x, R1: 0x%x, R2: 0x%x, R3: 0x%x\n\r------------------", state->pc, (state->fg & FG_ZERO) != 0 ? "FG_ZERO " : "", (state->fg & FG_EQ) != 0 ? "FG_EQ " : "", (state->fg & FG_OV) != 0 ? "FG_OV " : "", (state->fg & FG_HALT) != 0 ? "FG_HALT" : "", state->regs[0], state->regs[1], state->regs[2], state->regs[3]);
}

void core_run() {
	cpu_state_t state = { 0 };
	while (cpu_tick(&state)) {
		char out[0xff] = { 0 };
		cpu_dbg(&state, out);
		debugf("%s", out);
	}
}
