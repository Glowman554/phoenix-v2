#include <errors.h>
#include <stdio.h>
#include <stdlib.h>

void throw_error(char* str, bool err) {
	if (err) {
		fprintf(stderr, "%sERROR: %s%s\n", RED, str, COLOR_RESET);
		exit(1);
	} else
		;
}
