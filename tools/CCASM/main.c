#include <stdio.h>
#include <string.h>
#include <utils.h>
#include <lexer.h>
#include <parser.h>
#include <generator.h>

int main(void) {
    char* str = "add r1, 0xff";
    size_t len = strlen(str);
    int* tokens_len = malloc(sizeof(int));
    token_t* tokens = lex(str, len, tokens_len);
    print_tokens(*tokens_len, tokens);

    // parse
    init_labels(tokens, (size_t)(*tokens_len));
    root_t* ast = parse(tokens, (size_t)(*tokens_len), true);
    printf("opcode: 0x%x, r1: %d, r2: %d, imm: 0x%x, imm16: 0x%x\n", ast->nodes[0].opcode, ast->nodes[0].reg1, ast->nodes[0].reg2, ast->nodes[0].imm, ast->nodes[0].imm16);
    gen_from_instructions("out.bin", ast->nodes, ast->number_of_nodes);
    free(tokens_len);
    free(tokens);
}
