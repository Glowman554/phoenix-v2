#include <debug.h>
#include <stdio.h>
#include <stdarg.h>

void debugf(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	printf("\n");
}