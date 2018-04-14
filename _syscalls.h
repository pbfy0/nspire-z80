#ifndef _MALLOC_H
#define _MALLOC_H

#include <stdint.h>
#include <stddef.h>

void *_malloc(size_t size);
void _free(void *ptr);

#endif