#include <iostream>
#include <assert.h>
#include "lexer.h"
#include "generator.h"
#include "debug.h"

#ifdef _MSC_VER
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#endif

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

	char* code = (char*) malloc(size + 1);
	memset(code, 0, size + 1);
	fread(code, size, 1, f);

	fclose(f);

	lexer l = lexer((char*)code);

	assert(l.lex() == false);

	generator g = generator(l.get_tokens());
	assert(g.gen() == false);

	debugf("Succesfully assembled 0x%x bytes\n", g.get_data_len());

	debugf("Saving to %s...\n", out_file);
	f = fopen(out_file, "wb");
	assert(f != nullptr);

	fwrite(g.get_data(), g.get_data_len(), 1, f);
	fclose(f);

	free(code);
	return 0;
}