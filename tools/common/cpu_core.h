#pragma once

#include <stdint.h>
#include <cpu.h>
#include <cpu_disasm.h>
#include <stdbool.h>
#include <debug.h>
#include <stddef.h>

#define FG_ZERO (1 << 0)
#define FG_EQ (1 << 1)
#define FG_OV (1 << 2)
#define FG_HALT (1 << 3)

#ifdef SILENT
#define silent(s)
#else
#define silent(s) s
#endif

typedef struct cpu_state {
	uint16_t pc;
	uint16_t fg;
	uint8_t regs[16];

	uint8_t intr;
	uint8_t curr_intr;
	uint8_t intr_mask;
	uint16_t intr_ret;
	bool intr_lock;
} cpu_state_t;

uint8_t cpu_fetch_byte(uint16_t addr);
void cpu_write_byte(uint16_t addr, uint8_t val);

uint8_t cpu_io_read(uint16_t addr);
void cpu_io_write(uint16_t addr, uint8_t val);

static inline instruction_t cpu_fetch_instruction(uint16_t addr) {
	instruction_t instruction = { 0 };
	for (size_t i = 0; i < sizeof(instruction_t); i++) {
		((uint8_t*)&instruction)[i] = cpu_fetch_byte(addr + i);
	}

	return instruction;
}

static inline void cpu_write_instruction(instruction_t instr, uint16_t addr) {
	for (size_t i = 0; i < sizeof(instruction_t); i++) {
		cpu_write_byte(addr + i, ((uint8_t*)&instr)[i]);
	}
}

static inline uint8_t cpu_io_read_i(uint16_t addr, cpu_state_t* state) {
	if (addr == INT_IO) {
		return state->curr_intr;
	} else {
		return cpu_io_read(addr);
	}
}

static inline void cpu_io_write_i(uint16_t addr, uint8_t val, cpu_state_t* state) {
	if (addr == INT_IO) {
		state->intr_mask = val;
	} else {
		cpu_io_write(addr, val);
	}
}

static inline bool is_8bit_add_overflow(int a, int b) {
	return a + b > 0xff;
}

static inline bool is_8bit_sub_negative(int a, int b) {
	return a - b < 0;
}


static inline bool cpu_tick(cpu_state_t* state) {
	instruction_t instruction = cpu_fetch_instruction(state->pc);

	// debugf("0x%x: 0x%x 0x%x 0x%x 0x%x", state->pc, instruction.opcode, instruction.reg1, instruction.reg2, instruction.imm);

    silent({
    	char disassmebled_instr[32] = { 0 };
    	cpu_disasm(instruction, disassmebled_instr);
	    debugf("0x%x: %s", state->pc, disassmebled_instr);
    });

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
	case INSTR_OUTA:
		cpu_io_write_i(AR, state->regs[instruction.reg1], state);
		break;
	case INSTR_INPA:
		state->regs[instruction.reg1] = cpu_io_read_i(AR, state);
		break;
	case INSTR_JNZA:
		if ((state->fg & FG_ZERO) == 0) {
			silent(debugf("exec jmp"));
			state->pc = AR;
			goto out;
		}
		break;
	case INSTR_ADD:
		if (is_8bit_add_overflow(state->regs[instruction.reg1], state->regs[instruction.reg2])) {
			state->fg |= FG_OV;
			silent(debugf("setting bit FG_OV"));
		}
		state->regs[instruction.reg1] += state->regs[instruction.reg2];
		break;
	case INSTR_ADDI:
		if (is_8bit_add_overflow(state->regs[instruction.reg1], instruction.imm)) {
			state->fg |= FG_OV;
			silent(debugf("setting bit FG_OV"));
		}
		state->regs[instruction.reg1] += instruction.imm;
		break;
	case INSTR_SUB:
		if (is_8bit_sub_negative(state->regs[instruction.reg1], state->regs[instruction.reg2])) {
			state->fg |= FG_OV;
			silent(debugf("setting bit FG_OV"));
		}
		state->regs[instruction.reg1] -= state->regs[instruction.reg2];
		break;
	case INSTR_SUBI:
		if (is_8bit_sub_negative(state->regs[instruction.reg1], instruction.imm)) {
			state->fg |= FG_OV;
			silent(debugf("setting bit FG_OV"));
		}
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
			silent(debugf("setting bit FG_EQ"));
		}

		if (state->regs[instruction.reg1] == 0) {
			state->fg |= FG_ZERO;
			silent(debugf("setting bit FG_ZERO"));
		}
		break;
	case INSTR_CMPI:
		state->fg = 0;

		if (state->regs[instruction.reg1] == instruction.imm) {
			state->fg |= FG_EQ;
			silent(debugf("setting bit FG_EQ"));
		}

		if (state->regs[instruction.reg1] == 0) {
			state->fg |= FG_ZERO;
			silent(debugf("setting bit FG_ZERO"));
		}
		break;
	case INSTR_JZRA:
		if ((state->fg & FG_ZERO) != 0) {
			silent(debugf("exec jmp"));
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
			silent(debugf("exec jmp"));
			state->pc = AR;
			goto out;
		}
		break;
	case INSTR_JNQA:
		if ((state->fg & FG_EQ) == 0) {
			silent(debugf("exec jmp"));
			state->pc = AR;
			goto out;
		}
		break;
	case INSTR_JNZB:
		if ((state->fg & FG_ZERO) == 0) {
			silent(debugf("exec jmp"));
			state->pc = BR;
			goto out;
		}
		break;
	case INSTR_JZRB:
		if ((state->fg & FG_ZERO) != 0) {
			silent(debugf("exec jmp"));
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
			silent(debugf("exec jmp"));
			state->pc = BR;
			goto out;
		}
		break;
	case INSTR_JNQB:
		if ((state->fg & FG_EQ) == 0) {
			silent(debugf("exec jmp"));
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
			silent(debugf("exec jmp"));
			state->pc = instruction.imm16;
			goto out;
		}
		break;
	case INSTR_JNQI:
		if ((state->fg & FG_EQ) == 0) {
			silent(debugf("exec jmp"));
			state->pc = instruction.imm16;
			goto out;
		}
		break;
	case INSTR_JZRI:
		if ((state->fg & FG_ZERO) != 0) {
			silent(debugf("exec jmp"));
			state->pc = instruction.imm16;
			goto out;
		}
		break;
	case INSTR_JNZI:
		if ((state->fg & FG_ZERO) == 0) {
			silent(debugf("exec jmp"));
			state->pc = instruction.imm16;
			goto out;
		}
		break;
	case INSTR_LADA:
		state->regs[0] = instruction.imm16 & 0xff;
		state->regs[1] = (instruction.imm16 & 0xff00) >> 8;
		break;
	case INSTR_LADB:
		state->regs[2] = instruction.imm16 & 0xff;
		state->regs[3] = (instruction.imm16 & 0xff00) >> 8;
		break;
	case INSTR_OUTB:
		cpu_io_write_i(BR, state->regs[instruction.reg1], state);
		break;
	case INSTR_INPB:
		state->regs[instruction.reg1] = cpu_io_read_i(BR, state);
		break;
	case INSTR_IRE:
		state->pc = state->intr_ret;
		state->curr_intr = 0;
		state->intr_lock = false;
		goto out;
		break;
	case INSTR_INT:
		state->intr |= INT7;
		break;
	case INSTR_JOFA:
		if ((state->fg & FG_OV) != 0) {
			silent(debugf("exec jmp"));
			state->pc = AR;
			goto out;
		}
		break;
	case INSTR_JOFB:
		if ((state->fg & FG_OV) != 0) {
			silent(debugf("exec jmp"));
			state->pc = BR;
			goto out;
		}
		break;
	case INSTR_JOFI:
		if ((state->fg & FG_OV) != 0) {
			silent(debugf("exec jmp"));
			state->pc = instruction.imm16;
			goto out;
		}
		break;
	case INSTR_JNOA:
		if ((state->fg & FG_OV) == 0) {
			silent(debugf("exec jmp"));
			state->pc = AR;
			goto out;
		}
		break;
	case INSTR_JNOB:
		if ((state->fg & FG_OV) == 0) {
			silent(debugf("exec jmp"));
			state->pc = BR;
			goto out;
		}
		break;
	case INSTR_JNOI:
		if ((state->fg & FG_OV) == 0) {
			silent(debugf("exec jmp"));
			state->pc = instruction.imm16;
			goto out;
		}
		break;
	default:
		silent(debugf("unk instr setting halt flag"));
		state->fg |= FG_HALT;
		break;
	}

	state->pc += sizeof(instruction_t);

out:
	if (state->pc == 0xffff) {
		silent(debugf("pc == 0xffff setting halt flag"));
		state->fg |= FG_HALT;
	} else {
		if ((state->intr & state->intr_mask) != 0 && !state->intr_lock) {
			state->curr_intr = state->intr;
			state->intr = 0;
			state->intr_ret = state->pc;
			state->intr_lock = true;
			state->pc = INT_ENTRY;
		}
	}

	return !((state->fg & FG_HALT) != 0);
}

// #define EXTRA_DEBUG
static inline void cpu_dbg(cpu_state_t* state, char* out) {
	// sprintf(out, "---- CPU STATE ----\n\rPC: 0x%x\n\rFG: %s%s%s%s\n\rR0: 0x%x, R1: 0x%x, R2: 0x%x\n\rR3: 0x%x, R4: 0x%x, R5: 0x%x\n\rINT_RET: 0x%x\n\rCURR_INT: 0x%x\n\r-------------------", state->pc, (state->fg & FG_ZERO) != 0 ? "FG_ZERO " : "", (state->fg & FG_EQ) != 0 ? "FG_EQ " : "", (state->fg & FG_OV) != 0 ? "FG_OV " : "", (state->fg & FG_HALT) != 0 ? "FG_HALT" : "", state->regs[0], state->regs[1], state->regs[2], state->regs[3], state->regs[4], state->regs[5], state->intr_ret, state->curr_intr);
	char* regs[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15" };

	out += sprintf(out, "---- CPU STATE ----\n");

	out += sprintf(out, "PC: 0x%x\n", state->pc);

	out += sprintf(out, "FG: ");
	if ((state->fg & FG_ZERO) != 0) {
		out += sprintf(out, "FG_ZERO ");
	}
	if ((state->fg & FG_EQ) != 0) {
		out += sprintf(out, "FG_EQ ");
	}
	if ((state->fg & FG_OV) != 0) {
		out += sprintf(out, "FG_OV ");
	}
	if ((state->fg & FG_HALT) != 0) {
		out += sprintf(out, "FG_HALT ");
	}
	out += sprintf(out, "\n");


	for (int i = 0; i < 16; i++) {
		out += sprintf(out, "%s: 0x%x\n", regs[i], state->regs[i]);
	}

#ifdef EXTRA_DEBUG
	out += sprintf(out, "INTR_RET: 0x%x\n", state->intr_ret);
	out += sprintf(out, "CURR_INTR: 0x%x\n", state->curr_intr);
#endif

	out += sprintf(out, "-------------------\n");

	#warning find better way to do this
}

static inline void core_run() {
	cpu_state_t state = { 0 };
	while (cpu_tick(&state)) {
        silent({
            char out[0xff] = { 0 };
		    cpu_dbg(&state, out);
		    debugf("%s", out);
        });
	}
}
