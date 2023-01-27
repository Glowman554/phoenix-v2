#include <cpu.h>
#include <cpu_disasm.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <cpu_core.h>

uint8_t memory_[0xffff] = { 0 };

uint8_t cpu_fetch_byte(uint16_t addr) {
	debugf("fetching byte at 0x%x", addr);
	return memory_[addr];
}

void cpu_write_byte(uint16_t addr, uint8_t val) {
	debugf("writing byte 0x%x at 0x%x", val, addr);
	memory_[addr] = val;
}

uint8_t cpu_io_read(uint16_t addr) {
	debugf("reading byte from io at 0x%x", addr);
	return 0x0;
}

void cpu_io_write(uint16_t addr, uint8_t val) {
	debugf("writing byte 0x%x to io at 0x%x", val, addr);
}

void cpu_tout(bool val) {
	debugf("tout: %s", val ? "true" : "false");
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
	while (cpu_tick(&state)) {
		char out[0xfff] = { 0 };
		cpu_dbg(&state, out);
		debugf("%s", out);
	}
}
