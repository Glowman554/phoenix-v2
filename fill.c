#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 4096 * 64

int main() {
	char* buf = malloc(SIZE);
	memset(buf, 0xff, SIZE);

	FILE* f = fopen("fill.bin", "wb");
	fwrite(buf, SIZE, 1, f);
	fclose(f);

	return 0;
}