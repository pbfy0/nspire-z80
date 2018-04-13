//#pragma GCC optimize ("-O1")

#include "_malloc.h"
#include "c_syscall.h"
#include <syscall-list.h>

void *_malloc(size_t size) {
	return (void *)wa_syscall1(e_malloc, size);
}

void _free(void *ptr) {
	wa_syscall1(e_free, (intptr_t)ptr);
}
