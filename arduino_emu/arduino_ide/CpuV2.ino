#include <stdint.h>
#include <stdio.h>
#include <EEPROM.h>

#ifdef __GNUC__
#define PACK(__Declaration__) __Declaration__ __attribute__((__packed__))
#endif
#ifdef _MSC_VER
#pragma warning(disable : 4996)  //_CRT_SECURE_NO_WARNINGS
#define PACK(__Declaration__) __pragma(pack(push, 1)) __Declaration__ __pragma(pack(pop))
#endif


// X == implemented, - == not yet implemented

#define PUT_A_ADDR (1 << 0)  // X

#define PUT_IR0_DB (1 << 1)   // X
#define PUT_IR1_DB (1 << 2)   // X
#define SAVE_DB_IR0 (1 << 3)  // X
#define SAVE_DB_IR1 (1 << 4)  // X


#define PUT_IIMM_DB (1 << 5)  // X

#define SAVE_DB_ALUA (1 << 6)  // X
#define SAVE_DB_ALUB (1 << 7)  // X

#define ALU_ADD (1 << 8)  // X
#define ALU_CMP (1 << 9)  // X

#define PUT_ALUOUT_DB (1 << 10)  // X

#define COND_INVERT (1 << 11)  // X

#define SAVE_PCB_IF_Z (1 << 12)   // X
#define SAVE_PCB_IF_EQ (1 << 13)  // X
#define SAVE_PCB (1 << 14)        // X

#define SAVE_DB_MEM (1 << 15)  // X
#define PUT_MEM_DB (1 << 16)   // X

#define PC_FLUSH (1 << 17)       // X
#define PC_FLUSH_COND (1 << 18)  // X

#define ALU_SUB (1 << 19)   // X
#define ALU_NAND (1 << 20)  // X
#define ALU_NOR (1 << 21)   // X

#define IO_IN (1 << 22)   // -
#define IO_OUT (1 << 23)  // -

#define PUT_B_ADDR (1 << 24)  // X

#define PUT_IADR_ADDR (1 << 25)  // -

#define FINISH (1 << 31)  // X

#define MAX_INSTR 0xff
#define MAX_MICROCODE_STEP 0b111

#define MICROCODE_STEP_0 1
#define MICROCODE_STEP_1 2
#define MICROCODE_STEP_2 3
#define MICROCODE_STEP_3 4
#define MICROCODE_STEP_4 5
#define MICROCODE_STEP_5 6

#define INSTR_NOP 0x0  // X
#define INSTR_MOV 0x1  // X
#define INSTR_LOD 0x2  // X

#define INSTR_OUT 0x3  // X
#define INSTR_INP 0x4  // X

#define INSTR_JNZA 0x5  // X
#define INSTR_ADD 0x6   // X
#define INSTR_ADDI 0x7  // X

#define INSTR_SUB 0x8   // X
#define INSTR_SUBI 0x9  // X

#define INSTR_NAD 0xa   // X
#define INSTR_NADI 0xb  // X
#define INSTR_NOR 0xc   // X
#define INSTR_NORI 0xd  // X

#define INSTR_CMP 0xe   // X
#define INSTR_CMPI 0xf  // X

#define INSTR_JZRA 0x10  // X

#define INSTR_LDRA 0x11  // X
#define INSTR_WTRA 0x12  // X

#define INSTR_JMPA 0x13  // X
#define INSTR_JEQA 0x14  // X
#define INSTR_JNQA 0x15  // X

#define INSTR_JNZB 0x16  // X
#define INSTR_JZRB 0x17  // X
#define INSTR_JMPB 0x18  // X
#define INSTR_JEQB 0x19  // X
#define INSTR_JNQB 0x1a  // X

#define INSTR_LDRB 0x1b  // X
#define INSTR_WTRB 0x1c  // X

#define INSTR_JMPI 0x1d  // -
#define INSTR_JEQI 0x1e  // -
#define INSTR_JNQI 0x1f  // -
#define INSTR_JZRI 0x20  // -
#define INSTR_JNZI 0x21  // -

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

#define A 4
#define B 5

instruction_t INSTR(uint8_t opcode, uint8_t reg1, uint8_t reg2, uint8_t imm) {
  instruction_t instr = { 0 };
  instr.opcode = opcode;
  instr.reg1 = reg1;
  instr.reg2 = reg2;
  instr.imm = imm;
  return instr;
}

instruction_t INSTR_IMM16(uint8_t opcode, uint16_t imm16) {
  instruction_t instr = { 0 };
  instr.opcode = opcode;
  instr.imm16 = imm16;
  return instr;
}

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

void debugf(const char* fmt, ...) {
  char printf_buf[128] = { 0 };
  va_list args;
  int printed;

  va_start(args, fmt);
  printed = vsprintf(printf_buf, fmt, args);
  va_end(args);

  Serial.println(printf_buf);
}

void cpu_disasm(instruction_t instruction, char* out) {
  char* regs[] = { "r0", "r1", "r2", "r3" };

  switch (instruction.opcode) {
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

#define FG_ZERO (1 << 0)
#define FG_EQ (1 << 2)
#define FG_OV (1 << 3)
#define FG_HALT (1 << 4)


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

void cpu_write_instruction(instruction_t instr, uint16_t addr) {
  for (int i = 0; i < sizeof(instruction_t); i++) {
    cpu_write_byte(addr + i, ((uint8_t*)&instr)[i]);
  }
}

struct cpu_state_t {
  uint16_t pc;
  uint16_t fg;
  uint8_t regs[4];
};

bool cpu_tick(struct cpu_state_t* state) {
  instruction_t instruction = cpu_fetch_instruction(state->pc);

  // debugf("0x%x: 0x%x 0x%x 0x%x 0x%x", state->pc, instruction.opcode, instruction.reg1, instruction.reg2, instruction.imm);

  char disassmebled_instr[32] = { 0 };
  cpu_disasm(instruction, disassmebled_instr);
  debugf("0x%x: %s", state->pc, disassmebled_instr);

  uint16_t AR = (state->regs[1] << 8) | state->regs[0];
  uint16_t BR = (state->regs[3] << 8) | state->regs[2];

  switch (instruction.opcode) {
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

// -------------------------------------------------------------------------

uint8_t cpu_fetch_byte(uint16_t addr) {
  debugf("fetching byte at 0x%x", addr);
  return EEPROM.read(addr);
}

void cpu_write_byte(uint16_t addr, uint8_t val) {
  debugf("writing byte 0x%x at 0x%x", val, addr);
  EEPROM.write(addr, val);
}

uint8_t cpu_io_read(uint16_t addr) {
  debugf("reading byte from io at 0x%x", addr);
  return 0x0;
}

void cpu_io_write(uint16_t addr, uint8_t val) {
  debugf("writing byte 0x%x to io at 0x%x", val, addr);
}

// #define WRITE_PROGRAM

void setup() {
  Serial.begin(9600);

#ifdef WRITE_PROGRAM
  /* 0x0  */ cpu_write_instruction(LOD(R0, 0), 0x0);
  /* 0x3  */ cpu_write_instruction(LOD(R1, 1), 0x3);
  /* 0x6  */ cpu_write_instruction(LOD(R2, 12), 0x6); //  compute the 13th fibonacci number leaves the result in r3
  // loop:
  /* 0x9  */ cpu_write_instruction(MOV(R3, R0), 0x9);
  /* 0xc  */ cpu_write_instruction(ADD(R3, R1), 0xc);
  /* 0xf  */ cpu_write_instruction(MOV(R0, R1), 0xf);
  /* 0x12 */ cpu_write_instruction(MOV(R1, R3), 0x12);
  /* 0x15 */ cpu_write_instruction(SUBI(R2, 1), 0x15);
  /* 0x18 */ cpu_write_instruction(CMPI(R2, 0), 0x18);
  /* 0x1b */ cpu_write_instruction(JNZI(0x9), 0x1b); // jmp not zero to loop
  // done:
  /* 0x1e */ cpu_write_instruction(JMPI(0xffff), 0x1e); // jump to 0xffff halts the cpu
#endif

  cpu_state_t state = { 0 };
	while (cpu_tick(&state)) {
		char out[0xff] = { 0 };
		cpu_dbg(&state, out);
		debugf("%s", out);
	}
}

void loop() {
  // put your main code here, to run repeatedly:
}