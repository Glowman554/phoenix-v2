# Architecture

## Registers

The cpu contains 3 registers:

- R0 (AL)
- R1 (AH)
- R2 (BL)
- R3 (BH)

The R0 and R1 registers are used to store the low and high bytes of the A register used for addressing of memory and io. The same happens for R2 and R3 wich make the B register.
All registers are 8 bits wide.

## Instruction set

The instruction set contains instructions to add / substract / nand / nor / compare / read and write memory and jump. Jumps and memory accesses can only be done using the A register.

These instructions are implemented in the microcode.
The cpu uses a multi-step microcode execution engine to execute those instructions.

The instructions are always 3 bytes long.
The instructions are encoded in the following way:

- first 8bit = instruction
- next 4bit  = first optional register
- next 4bit  = second optional register
- last 8bit = optional imm8

## IO

The io can vary depending on the implementation. Generally, there are 2 control lines called IO_IN and IO_OUT which are used to control the IO device.
The IO device should read the value of the address bus and depending on the control lines, write the value to the data bus or read the value from the data bus.

## Memory

This cpu is a Von Neumann architecture. It can execute instructions either from ROM or RAM.

The recommended memory layout is:

- 0x0000 - 0x7fff = general purpose rom
- 0x8000 - 0xffff = general purpose ram

The cpu halts the execution when the program counter gets set to 0xffff either from reaching the end of the memory space or from an JMP instruction loading 0xffff into the program counter.
