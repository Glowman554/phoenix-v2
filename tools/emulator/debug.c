#include <debug.h>
#include <stdarg.h>
#include <stdio.h>

void debugf(const char* fmt, ...) {
    char print_buf[1024] = { 0 };
    va_list args;
    va_start(args, fmt);
    vsprintf(print_buf, fmt, args);
    va_end(args);
    printf("%s\n", print_buf);
}

