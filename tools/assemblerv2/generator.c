#include <generator.h>
#include <stdio.h>
#include <string.h>

bool endsWith(const char* str, const char* ending) {
    int strLen = strlen(str);
    int endingLen = strlen(ending);

    if (endingLen > strLen) {
        return false;
	}

    return strncmp(str + strLen - endingLen, ending, endingLen) == 0;
}

void gen_from_instructions(char* file_name, instruction_t* instructions, size_t instructions_len) {
	FILE* f;

	if (endsWith(file_name, ".bin")) {
		f = fopen(file_name, "wb");
		fwrite((void*)instructions, sizeof(instruction_t) * instructions_len, 1, f);
	} else if (endsWith(file_name, ".hex")) {
		f = fopen(file_name, "w");

		fputs("@0\n", f);

		uint8_t* buffer = (uint8_t*) instructions;

		for (int i = 0; i < sizeof(instruction_t) * instructions_len; i++) {
			fprintf(f, "%02x\n", buffer[i]);
		}
	} else {
		printf("Invalid ouput format selected for file %s!\n", file_name);
		abort();
	}

	fclose(f);
}