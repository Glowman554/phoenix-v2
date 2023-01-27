#include <assert.h>
#include "lexer.h"
#include "generator.h"
#include "debug.h"
#include <preprocessor.h>

#ifdef _MSC_VER
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#endif

char* code;

void print_error(int pos_start, int pos_end, char* error) {
	printf("error: %s\n", error);

	int i = pos_start;
	while (code[i] != '\n' && i >=s 0) {
		i--;
	}

	for (int j = i + 1; code[j] != '\n' && code[j] != '\0'; j++) {
		if (code[j] == '\t') {
			printf(" ");
		}
		else {
			printf("%c", code[j]);
		}
	}
	printf("\n");

	for (int j = 0; j < pos_start - i - 1; j++) {
		printf(" ");
	}
	for (int j = 0; j < pos_end - pos_start; j++) {
		printf("^");
	}
	printf("\n");
}

int main(int argc, char** argv) {
	if (argc != 2 && argc != 3) {
		printf("usage: %s <in_file> <out_file?>\n", argv[0]);
		return 1;
	}

	char* in_file = argv[1];

	char* out_file = (char*) "rom.bin";
	if (argc == 3) {
		out_file = argv[2];
	}

	debugf("Loading %s...\n", in_file);
	FILE* f = fopen(in_file, "r");
	assert(f != nullptr);

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	code = (char*) malloc(size + 1);
	memset(code, 0, size + 1);
	fread(code, size, 1, f);

	fclose(f);

	char* old_code = code;
	code = preprocess(code);
	free(old_code);

	lexer l = lexer((char*)code);

	assert(l.lex() == false);

	generator g = generator(l.get_tokens());
	assert(g.gen() == false);

	debugf("Successfully assembled 0x%x bytes\n", g.get_data_len());

	debugf("Saving to %s...\n", out_file);
	f = fopen(out_file, "wb");
	assert(f != nullptr);

	fwrite(g.get_data(), g.get_data_len(), 1, f);
	fclose(f);

	free(code);
	return 0;
}
