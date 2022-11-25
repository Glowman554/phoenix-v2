#include <generator.h>
#include <stdio.h>

void gen_from_instructions(char* file_name, instruction_t* instructions, size_t instructions_len) {
    FILE* f;
    f = fopen(file_name, "wb");

    fwrite((void*)instructions, sizeof(instruction_t) * instructions_len, 1, f);
    fclose(f);
}