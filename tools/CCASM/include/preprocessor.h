#pragma once

#include <stdint.h>
#include <stdlib.h>

#define BUFF_SIZE 0xffff
#define IMPORT_LENGTH 7
#define END_OF_IMPORT_LENGTH 1

// has to be allocated. Has to be freed by the user as well
void preprocess(char* code, size_t* code_len);
