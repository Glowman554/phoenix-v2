// #define __AVR_ATmega328P__

#include <avr/io.h>
#include <usart.h>
#include <debug.h>
#include <avr/eeprom.h> 
#include <core.h>
#include <program.h>

uint8_t cpu_fetch_byte(uint16_t addr) {
	debugf("fetching byte at 0x%x", addr);

	return eeprom_read_byte((uint8_t*) addr);
}

void cpu_write_byte(uint16_t addr, uint8_t val) {
	debugf("writing byte 0x%x at 0x%x", val, addr);

	// this check is done since the eeprom has a limited amount of writes
	if (eeprom_read_byte((uint8_t*) addr) == val) {
		// debugf("no need to write again! *0x%x == 0x%x", addr, val);
	} else {
		eeprom_write_byte((uint8_t*) addr, val);
	}
}

uint8_t cpu_io_read(uint16_t addr) {
	debugf("reading byte from io at 0x%x", addr);
	return 0x0;
}

void cpu_io_write(uint16_t addr, uint8_t val) {
	debugf("writing byte 0x%x to io at 0x%x", val, addr);
}

void main() {
	DDRB |= _BV(DDB5);

	USART0_init();

	write_program_to_eeprom();
	core_run();

	PORTB |= _BV(DDB5);

	while (1) {
	}
}
