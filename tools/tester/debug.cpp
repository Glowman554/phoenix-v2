extern "C" {
	#include <debug.h>
}

#include <stdarg.h>
#include <stdio.h>

FILE* logfile = nullptr;

extern "C" {
	void debugf(const char* fmt, ...) {
		if (logfile != nullptr) {
			va_list args;
			va_start(args, fmt);
			vfprintf(logfile, fmt, args);
			fprintf(logfile, "\n");
			va_end(args);
		}
	}
}