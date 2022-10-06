#pragma once

#include <stdint.h>

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif
#ifdef _MSC_VER
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif


// X == implemented, - == not yet implemented

#define PUT_A_ADDR		(1 << 0) // X

#define PUT_IR0_DB		(1 << 1) // X
#define PUT_IR1_DB		(1 << 2) // X
#define SAVE_DB_IR0		(1 << 3) // X
#define SAVE_DB_IR1		(1 << 4) // X


#define PUT_IIMM_DB		(1 << 5) // X

#define SAVE_DB_ALUA	(1 << 6) // X
#define SAVE_DB_ALUB	(1 << 7) // X

#define ALU_ADD			(1 << 8) // X
#define ALU_CMP			(1 << 9) // X

#define PUT_ALUOUT_DB	(1 << 10) // X

#define COND_INVERT		(1 << 11) // X

#define SAVE_PCB_IF_Z	(1 << 12) // X
#define SAVE_PCB_IF_EQ	(1 << 13) // X
#define SAVE_PCB		(1 << 14) // X

#define SAVE_DB_MEM		(1 << 15) // X
#define PUT_MEM_DB		(1 << 16) // X

#define PC_FLUSH		(1 << 17) // X
#define PC_FLUSH_COND	(1 << 18) // X

#define ALU_SUB			(1 << 19) // X
#define ALU_NAND		(1 << 20) // X
#define ALU_NOR			(1 << 21) // X

#define IO_IN			(1 << 22) // -
#define IO_OUT			(1 << 23) // -

#define PUT_B_ADDR		(1 << 24) // X

#define PUT_IADR_ADDR	(1 << 25) // -

#define FINISH			(1 << 31) // X

#define MAX_INSTR 0xff
#define MAX_MICROCODE_STEP 0b111

#define MICROCODE_STEP_0 1
#define MICROCODE_STEP_1 2
#define MICROCODE_STEP_2 3
#define MICROCODE_STEP_3 4
#define MICROCODE_STEP_4 5
#define MICROCODE_STEP_5 6

#define INSTR_NOP 0x0	// X
#define INSTR_MOV 0x1	// X
#define INSTR_LOD 0x2	// X

#define INSTR_OUT 0x3	// X
#define INSTR_INP 0x4	// X

#define INSTR_JNZA 0x5	// X
#define INSTR_ADD 0x6	// X
#define INSTR_ADDI 0x7	// X

#define INSTR_SUB 0x8	// X
#define INSTR_SUBI 0x9	// X

#define INSTR_NAD 0xa	// X
#define INSTR_NADI 0xb	// X
#define INSTR_NOR 0xc	// X
#define INSTR_NORI 0xd	// X

#define INSTR_CMP 0xe	// X
#define INSTR_CMPI 0xf	// X

#define INSTR_JZRA 0x10	// X

#define INSTR_LDRA 0x11	// X
#define INSTR_WTRA 0x12	// X

#define INSTR_JMPA 0x13	// X
#define INSTR_JEQA 0x14	// X
#define INSTR_JNQA 0x15	// X

#define INSTR_JNZB 0x16	// X
#define INSTR_JZRB 0x17	// X
#define INSTR_JMPB 0x18	// X
#define INSTR_JEQB 0x19	// X
#define INSTR_JNQB 0x1a	// X

#define INSTR_LDRB 0x1b	// X
#define INSTR_WTRB 0x1c	// X

#define INSTR_JMPI 0x1d	// -
#define INSTR_JEQI 0x1e	// -
#define INSTR_JNQI 0x1f	// -
#define INSTR_JZRI 0x20	// -
#define INSTR_JNZI 0x21	// -

typedef PACK(struct instruction {
	uint8_t opcode;
	union {
		struct {
			uint8_t reg1 : 4;
			uint8_t reg2 : 4;
			uint8_t imm;
		};
		uint16_t imm16;
	};
}) instruction_t;

#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define R5 5

#define A 16
#define B 17

#define INSTR(opcode_, reg1_, reg2_, imm_) ((instruction_t) { .opcode = opcode_, .reg1 = reg1_, .reg2 = reg2_, .imm = imm_ })
#define INSTR_IMM16(opcode_, imm16_) ((instruction_t) { .opcode = opcode_, .imm16 = imm16_ })

#define NOP() INSTR(INSTR_NOP, 0, 0, 0)
#define MOV(reg1, reg2) INSTR(INSTR_MOV, reg1, reg2, 0)
#define LOD(reg, imm) INSTR(INSTR_LOD, reg, 0, imm)

#define OUT(a, reg) INSTR(INSTR_OUT, reg, 0, 0)
#define INP(reg, a) INSTR(INSTR_INP, reg, 0, 0)

#define JNZ(a) a == A ? INSTR(INSTR_JNZA, 0, 0, 0) : INSTR(INSTR_JNZB, 0, 0, 0)
#define ADD(reg1, reg2) INSTR(INSTR_ADD, reg1, reg2, 0)
#define ADDI(reg1, imm) INSTR(INSTR_ADDI, reg1, 0, imm)

#define SUB(reg1, reg2) INSTR(INSTR_SUB, reg1, reg2, 0)
#define SUBI(reg1, imm) INSTR(INSTR_SUBI, reg1, 0, imm)

#define NAD(reg1, reg2) INSTR(INSTR_NAD, reg1, reg2, 0)
#define NADI(reg1, imm) INSTR(INSTR_NADI, reg1, 0, imm)
#define NOR(reg1, reg2) INSTR(INSTR_NOR, reg1, reg2, 0)
#define NORI(reg1, imm) INSTR(INSTR_NORI, reg1, 0, imm)


#define CMP(reg1, reg2) INSTR(INSTR_CMP, reg1, reg2, 0)
#define CMPI(reg1, imm) INSTR(INSTR_CMPI, reg1, 0, imm)

#define JZR(a) a == A ? INSTR(INSTR_JZRA, 0, 0, 0) : INSTR(INSTR_JZRB, 0, 0, 0)

#define LDR(reg, a) a == A ? INSTR(INSTR_LDRA, reg, 0, 0) : INSTR(INSTR_LDRB, reg, 0, 0)
#define WTR(a, reg) a == A ? INSTR(INSTR_WTRA, reg, 0, 0) : INSTR(INSTR_WTRB, reg, 0, 0)

#define JMP(a) a == A ? INSTR(INSTR_JMPA, 0, 0, 0) : INSTR(INSTR_JMPB, 0, 0, 0)
#define JEQ(a) a == A ? INSTR(INSTR_JEQA, 0, 0, 0) : INSTR(INSTR_JEQB, 0, 0, 0)
#define JNQ(a) a == A ? INSTR(INSTR_JNQA, 0, 0, 0) : INSTR(INSTR_JNQB, 0, 0, 0)

#define JMPI(imm) INSTR_IMM16(INSTR_JMPI, imm)
#define JEQI(imm) INSTR_IMM16(INSTR_JEQI, imm)
#define JNQI(imm) INSTR_IMM16(INSTR_JNQI, imm)
#define JZRI(imm) INSTR_IMM16(INSTR_JZRI, imm)
#define JNZI(imm) INSTR_IMM16(INSTR_JNZI, imm)
