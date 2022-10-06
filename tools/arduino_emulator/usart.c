#include <usart.h>

// #define __AVR_ATmega328P__

#include <avr/io.h>
#include <util/setbaud.h>

void USART0_init() {
	UBRR0H = (unsigned char) (UBRR_VALUE >> 8);
	UBRR0L = (unsigned char) UBRR_VALUE;

	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Set frame format: 8data, 2stop bit
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void USART0_transmit(unsigned char data) {
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)));
	
	// Put data into buffer, sends the data
	UDR0 = data;
}

void USART0_transmit_str(const char* str) {
	while (*str) {
		USART0_transmit(*str++);
	}
}

unsigned char USART0_receive() {
    // Wait for data to be received
    while (!(UCSR0A & (1 << RXC0)));

    // Get and return received data from buffer
    return UDR0;
}

int USART0_receive_until(unsigned char* out, char delim) {
    int bytes = 0;
    char buf = 0;

    while ((buf = USART0_receive()) != delim) {
        out[bytes++] = buf;
    }

    return bytes;
}
