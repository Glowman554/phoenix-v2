#include <stdio.h>
#include <string.h>
#include "include/lexer.h"
#include "include/utils.h"
#include "include/parser.h"

int main(void) {
    char* str = "notmain:nop nop nop";
    size_t len = strlen(str);
    int* tokens_len = malloc(sizeof(size_t));
    token_t* tokens = lex(str, len, tokens_len);
    print_tokens(*tokens_len, tokens);

    // parse
    init_labels(tokens, (size_t)(*tokens_len));
    parse(tokens, (size_t)(*tokens_len), true);
    free(tokens_len);
    free(tokens);
}
