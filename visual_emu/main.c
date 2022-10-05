#include <core.h>
#include <debug.h>
#include <stdbool.h>
#include <stdio.h>
#include <cpu_disasm.h>
#include <stdlib.h>

uint8_t memory_[0xffff] = { 0 };

uint8_t cpu_fetch_byte(uint16_t addr) {
	debugf("fetching byte at 0x%x\n", addr);
	return memory_[addr];
}

void cpu_write_byte(uint16_t addr, uint8_t val) {
	debugf("writing byte 0x%x at 0x%x\n", val, addr);
	memory_[addr] = val;
}

uint8_t cpu_io_read(uint16_t addr) {
	debugf("reading byte from io at 0x%x\n", addr);
	return 0x0;
}

void cpu_io_write(uint16_t addr, uint8_t val) {
	debugf("writing byte 0x%x to io at 0x%x\n", val, addr);
}

#define DISASEMBLY_SIZE 6

void do_disasm(cpu_state_t state) {
    uint16_t start_disassembly = state.pc - sizeof(instruction_t) * DISASEMBLY_SIZE;
    for (int i = 0; i < DISASEMBLY_SIZE * 2 + 1; i++) {
        char disasm[0xff] = { 0 };
        cpu_disasm(cpu_fetch_instruction(start_disassembly + sizeof(instruction_t) * i), disasm);

        char buf[0xff * 2] = { 0 };
        sprintf(buf, "%4x: %s", (uint16_t) (start_disassembly + sizeof(instruction_t) * i), disasm);
        printf("%s %s\n", i == DISASEMBLY_SIZE ? "->" : "  ", buf);
    }
}

char getch() {
    system("stty raw");
    char c = getc(stdin);
    system("stty cooked");
    return c;
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

	if (size > sizeof(memory_)) {
		printf("file too large\n");
		return 1;
	}

	fread(memory_, 1, size, f);
	fclose(f);

    cpu_state_t state = { 0 };

    system("clear");
    printf("Welcome to the %s emulator!\nPress 'h' for help.\n", argv[0]);

    bool running = true;
    while (running) {
        printf(">");

        char command = getch();
        system("clear");

        switch (command) {
            case 'h':
                {
                    printf("h -> help\n");
                    printf("s -> step\n");
                    printf("q -> quit\n");
                    printf("d -> dump\n");
                }
                break;

            case 'q':
                {
                    running = false;
                }
                break;

            case 's':
                {
                    running = cpu_tick(&state);

                    char dbg[0xff] = { 0 };
                    cpu_dbg(&state, dbg);

                    printf("%s\n\n", dbg);
                    do_disasm(state);
                }
                break;

            case 'd':
                {
                    char dbg[0xff] = { 0 };
                    cpu_dbg(&state, dbg);

                    printf("%s\n\n", dbg);
                    do_disasm(state);
                }
                break;

            case '\n':
                break;

            default:
                {
                    printf("Unknown command %c!\n", command);
                }
                break;
        }
    }

}
