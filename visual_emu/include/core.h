#pragma once

#include <stdint.h>
#include <cpu.h>
#include <stdbool.h>

#define FG_ZERO (1 << 0)
#define FG_EQ (1 << 2)
#define FG_OV (1 << 3)
#define FG_HALT (1 << 4)

typedef struct cpu_state {
	uint16_t pc;
	uint16_t fg;
	uint8_t regs[4];
} cpu_state_t;

uint8_t cpu_fetch_byte(uint16_t addr);
void cpu_write_byte(uint16_t addr, uint8_t val);

uint8_t cpu_io_read(uint16_t addr);
void cpu_io_write(uint16_t addr, uint8_t val);

instruction_t cpu_fetch_instruction(uint16_t addr);
void cpu_write_instruction(instruction_t instr, uint16_t addr);

bool cpu_tick(cpu_state_t* state);
void cpu_dbg(cpu_state_t* state, char* out);

void core_run();