#ifndef _MALLOC_H
#define _MALLOC_H

#include <stdint.h>
#include <stddef.h>
#include <libndls.h>

void *_malloc(size_t size);
void _free(void *ptr);
scr_type_t _lcd_type();
size_t real_puts(const char *ptr);

#endif