#include <assert.h>
#include <generator.h>
#include <lexer.h>
#include <parser.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
	assert(argc > 1);

	char* str;
	FILE* f = fopen(argv[1], "r");
	assert(f != NULL);

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	str = (char*)malloc(size + 1);
	memset(str, 0, size + 1);
	fread(str, size, 1, f);

	fclose(f);

	size_t len = strlen(str);
	int* tokens_len = malloc(sizeof(int));
	token_t* tokens = lex(str, len, tokens_len);

	// parse
	init_labels(tokens, (size_t)(*tokens_len));
	root_t* ast = parse(tokens, (size_t)(*tokens_len), true);
	gen_from_instructions("out.bin", ast->nodes, ast->number_of_nodes);
	free(tokens_len);
	free(tokens);
}
