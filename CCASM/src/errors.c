#include "include/errors.h"
#include <stdio.h>
#include <stdlib.h>

void throw_error(char* str, bool quit) {
    printf("%sERROR: %s%s\n", RED, str, COLOR_RESET);
    if (quit) {
        exit(1);
    }
}
