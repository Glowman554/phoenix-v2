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

#define IO_IN			(1 << 22) // x
#define IO_OUT			(1 << 23) // x

#define PUT_B_ADDR		(1 << 24) // X

#define PUT_IADR_ADDR	(1 << 25) // x

#define SAVE_ADDR_A		(1 << 26) // x
#define SAVE_ADDR_B		(1 << 27) // x

#define INT_RET_TO_ADDR (1 << 28)
#define INT7_TRIGGER	(1 << 29)
#define FINISH			(1 << 31) // X

// mc2
#define SAVE_PCB_IF_OF 	(1 << 0)
#define CLEAR_FG		(1 << 1)
#define ALU_ADO			(1 << 2)
#define ALU_SBO			(1 << 3)

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

#define INSTR_OUTA 0x3	// X
#define INSTR_INPA 0x4	// X

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

#define INSTR_JMPI 0x1d	// x
#define INSTR_JEQI 0x1e	// x
#define INSTR_JNQI 0x1f	// x
#define INSTR_JZRI 0x20	// x
#define INSTR_JNZI 0x21	// x

#define INSTR_LADA 0x22 // x
#define INSTR_LADB 0x23 // x

#define INSTR_OUTB 0x24	// X
#define INSTR_INPB 0x25	// X

#define INSTR_IRE 0x26

#define INSTR_INT 0x27

#define INSTR_JOFA 0x28
#define INSTR_JOFB 0x29
#define INSTR_JOFI 0x2a

#define INSTR_JNOA 0x2b
#define INSTR_JNOB 0x2c
#define INSTR_JNOI 0x2d

#define INSTR_ADO 0x2e
#define INSTR_ADOI 0x2f

#define INSTR_SBO 0x30
#define INSTR_SBOI 0x31

#define INSTR_CFG 0x32

#define INT0	(1 << 0)
#define INT1	(1 << 1)
#define INT2	(1 << 2)
#define INT3	(1 << 3)
#define INT4	(1 << 4)
#define INT5	(1 << 5)
#define INT6	(1 << 6)
#define INT7	(1 << 7)

#define INT_ENTRY 0x8000
#define INT_IO 0xff00

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

#define R0 0 // x 0b0000
#define R1 1 // x 0b0001
#define R2 2 // x 0b0010
#define R3 3 // x 0b0011
#define R4 4 // x 0b0100
#define R5 5 // x 0b0101

#define R6 6 // x 0b0110
#define R7 7 // x 0b0111
#define R8 8 // x 0b1000
#define R9 9 // x 0b1001
#define R10 10 // x 0b1010
#define R11 11 // x 0b1011
#define R12 12 // x 0b1100
#define R13 13 // - 0b1101
#define R14 14 // - 0b1110
#define R15 15 // - 0b1111

#define A 16
#define B 17

#define INSTR(opcode_, reg1_, reg2_, imm_) ((instruction_t) { .opcode = opcode_, .reg1 = reg1_, .reg2 = reg2_, .imm = imm_ })
#define INSTR_IMM16(opcode_, imm16_) ((instruction_t) { .opcode = opcode_, .imm16 = imm16_ })

#define NOP() INSTR(INSTR_NOP, 0, 0, 0)
#define MOV(reg1, reg2) INSTR(INSTR_MOV, reg1, reg2, 0)
#define LOD(reg, imm) INSTR(INSTR_LOD, reg, 0, imm)

#define OUT(a, reg) a == A ? INSTR(INSTR_OUTA, reg, 0, 0) : INSTR(INSTR_OUTB, reg, 0, 0)
#define INP(reg, a) a == A ? INSTR(INSTR_INPA, reg, 0, 0) : INSTR(INSTR_INPB, reg, 0, 0)

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

#define LAD(a, imm) a == A ? INSTR_IMM16(INSTR_LADA, imm) : INSTR_IMM16(INSTR_LADB, imm)

#define IRE() INSTR(INSTR_IRE, 0, 0, 0)
#define INT() INSTR(INSTR_INT, 0, 0, 0)

#define JOF(a) a == A ? INSTR(INSTR_JOFA, 0, 0, 0) : INSTR(INSTR_JOFB, 0, 0, 0)
#define JOFI(imm) INSTR_IMM16(INSTR_JOFI, imm)

#define JNO(a) a == A ? INSTR(INSTR_JNOA, 0, 0, 0) : INSTR(INSTR_JNOB, 0, 0, 0)
#define JNOI(imm) INSTR_IMM16(INSTR_JNOI, imm)

#define ADO(reg1, reg2) INSTR(INSTR_ADO, reg1, reg2, 0)
#define ADOI(reg1, imm) INSTR(INSTR_ADOI, reg1, 0, imm)

#define SBO(reg1, reg2) INSTR(INSTR_SBO, reg1, reg2, 0)
#define SBOI(reg1, imm) INSTR(INSTR_SBOI, reg1, 0, imm)

#define CFG() INSTR(INSTR_CFG, 0, 0, 0)
