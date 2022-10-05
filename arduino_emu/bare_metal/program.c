#include <program.h>
#include <cpu.h>
#include <core.h>

void write_program_to_eeprom() {
	cpu_write_instruction(LOD(R0, 0), 0 * sizeof(instruction_t));
	cpu_write_instruction(LOD(R1, 1), 1 * sizeof(instruction_t));
	cpu_write_instruction(LOD(R2, 12), 2 * sizeof(instruction_t)); //  compute the 13th fibonacci number leaves the result in r3
	// loop:
	cpu_write_instruction(MOV(R3, R0), 3 * sizeof(instruction_t));
	cpu_write_instruction(ADD(R3, R1), 4 * sizeof(instruction_t));
	cpu_write_instruction(MOV(R0, R1), 5 * sizeof(instruction_t));
	cpu_write_instruction(MOV(R1, R3), 6 * sizeof(instruction_t));
	cpu_write_instruction(SUBI(R2, 1), 7 * sizeof(instruction_t));
	cpu_write_instruction(CMPI(R2, 0), 8 * sizeof(instruction_t));
	cpu_write_instruction(JNZI(3 * sizeof(instruction_t)), 9 * sizeof(instruction_t)); // jmp not zero to loop
	// done:
	cpu_write_instruction(JMPI(0xffff), 10 * sizeof(instruction_t)); // jump to 0xffff halts the cpu
}