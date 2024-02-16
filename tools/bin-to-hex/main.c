#include <assert.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    assert(argc == 3);

    FILE* input = fopen(argv[1], "rb");
    FILE* output = fopen(argv[2], "w");

	fputs("@0\n", output);

    unsigned char b = 0;
    while (fread(&b, 1, 1, input)) {
        fprintf(output, "%x\n", b);
    }

    fclose(input);
    fclose(output);

    return 0;
}