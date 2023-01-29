#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include <cpu_core.h>

#include <stdarg.h>

#include <programing.h>

uint8_t memory_[0xffff] = { 0 };

uint8_t cpu_fetch_byte(uint16_t addr) {
	return memory_[addr];
}

void cpu_write_byte(uint16_t addr, uint8_t val) {
	memory_[addr] = val;
}

uint8_t cpu_io_read(uint16_t addr) {
	switch (addr) {
		case 0x0:
			{
				uint8_t val = 0;
				for (int i = 0; i < 8; i++) {
					val |= gpio_get(i + 8) << i;
				}
				return val;
			}
		default:
			silent(debugf("invalid io address %x", addr));
			return 0x0;
	}

}

void cpu_io_write(uint16_t addr, uint8_t val) {
	
	switch (addr) {
		case 0x0:
			for (int i = 0; i < 8; i++) {
				gpio_put(i, (val & 1 << i) ? true : false);
			}
			return;
		default:
			silent(debugf("invalid io address %x", addr));
			return;
	}
}

uint16_t current_emit = 0;
uint16_t emit(instruction_t instr) {
	cpu_write_instruction(instr, current_emit);
	uint16_t result = current_emit;
	current_emit += sizeof(instruction_t);
	return result;
}

#define INT0_IN 28
#define INT1_IN 27
#define INT2_IN 26
#define INT3_IN 22
#define INT4_IN 21
#define INT5_IN 20

#define TOUT 18

void cpu_tout(bool val) {
	gpio_put(TOUT, val);
}

cpu_state_t state = { 0 };


void int_callback(uint gpio, uint32_t events) {
	switch (gpio) {
		case INT0_IN:
			state.intr |= INT0;
			break;
		case INT1_IN:
			state.intr |= INT1;
			break;
		case INT2_IN:
			state.intr |= INT2;
			break;
		case INT3_IN:
			state.intr |= INT3;
			break;
		case INT4_IN:
			state.intr |= INT4;
			break;
		case INT5_IN:
			state.intr |= INT5;
			break;
	}
}

void _core_run() {
	while (cpu_tick(&state)) {
		silent({
			char out[0xff] = { 0 };
			cpu_dbg(&state, out);
			debugf("%s", out);
		});
	#ifdef SLOW
		sleep_us(1000 * 1000);
	#endif
	}
}

void int_in_config(uint pin) {
	gpio_init(pin);
	gpio_set_dir(pin, GPIO_IN);
	gpio_set_pulls(pin, false, true);
	gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_RISE, true, &int_callback);
}

int main() {
	stdio_init_all();

	for (int i = 0; i < 8; i++) {
		gpio_init(i);
		gpio_set_dir(i, GPIO_OUT);
	}

	for (int i = 0; i < 8; i++) {
		gpio_init(i + 8);
		gpio_set_dir(i + 8, GPIO_IN);
	}

	gpio_init(TOUT);
	gpio_set_dir(TOUT, GPIO_OUT);

	int_in_config(INT0_IN);
	int_in_config(INT1_IN);
	int_in_config(INT2_IN);
	int_in_config(INT3_IN);
	int_in_config(INT4_IN);
	int_in_config(INT5_IN);


	programing_mode();

	_core_run();

	while (true) {}
}
