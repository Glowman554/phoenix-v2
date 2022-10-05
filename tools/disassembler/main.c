#include <cpu_disasm.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("usage: %s <file>\n", argv[0]);
		return 1;
	}

	FILE* f = fopen(argv[1], "rb");
	if (f == NULL) {
		printf("could not open file\n");
		return 1;
	}

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	instruction_t* instructions = (instruction_t*) malloc(size);

	fread(instructions, 1, size, f);
	fclose(f);

	for (int i = 0; i < size / sizeof(instruction_t); i++) {
		char out[256];
		cpu_disasm(instructions[i], out);
		printf("0x%lx:\t%s\n", i * sizeof(instruction_t), out);
	}
}
