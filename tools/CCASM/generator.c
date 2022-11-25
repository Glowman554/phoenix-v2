#include <generator.h>
#include <stdio.h>

void gen_from_instructions(char* file_name, instruction_t* instructions, size_t instructions_len) {
    FILE* f;
    f = fopen(file_name, "wb");

    for (int i = 0; i < instructions_len; i++) {
        fwrite((void*)&instructions[i], sizeof(instruction_t), 1, f);
    }
    fclose(f);
}