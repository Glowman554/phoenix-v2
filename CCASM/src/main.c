#include <stdio.h>
#include <string.h>
#include "include/lexer.h"

int main(void) {
    char* str = "100";
    size_t len = strlen(str);
    size_t tokens_len = 0;
    lex(str, len, &tokens_len);
}