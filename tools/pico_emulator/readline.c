#include <readline.h>
#include <stdio.h>
#include <string.h>

unsigned char str[MAX_LENGTH + 1];

unsigned char* readLine() {
	memset(str, 0, sizeof(str));
	unsigned char u;
	unsigned char* p;

	for(p = str, u = getchar(); u != '\n' && p - str < MAX_LENGTH; u = getchar()) {
		*p++ = u;
	}

	*p = 0;
	
	return str;
}
