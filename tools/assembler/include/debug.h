#pragma once

#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define debugf(...) printf("[%s:%d] ", __FILE__, __LINE__); printf(__VA_ARGS__)
#else
#define debugf(...)
#endif