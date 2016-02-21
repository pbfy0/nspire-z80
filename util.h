#pragma once
#include <assert.h>

#define FWRITE_VALUE(x, file) assert(fwrite(&(x), sizeof(x), 1, file) == 1)
#define FREAD_VALUE(x, file) assert(fread(x, sizeof(*(x)), 1, file) == 1)