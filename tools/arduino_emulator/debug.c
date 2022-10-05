#include <debug.h>

#include <stdarg.h>
#include <stdio.h>
#include <usart.h>

void debugf(const char* fmt, ...) {
	char printf_buf[128] = { 0 };
	va_list args;
	int printed;

	va_start(args, fmt);
	printed = vsprintf(printf_buf, fmt, args);
	va_end(args);

	USART0_transmit_str(printf_buf);
	USART0_transmit_str("\n\r");
}