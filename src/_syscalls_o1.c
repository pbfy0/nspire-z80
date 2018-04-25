//#pragma GCC optimize ("-O1")

#include "_syscalls.h"
#include "c_syscall.h"
#include <syscall-list.h>

void *_malloc(size_t size) {
	return (void *)wa_syscall1(e_malloc, size);
}

void _free(void *ptr) {
	wa_syscall1(e_free, (uintptr_t)ptr);
}

scr_type_t _lcd_type(){
	return wa_syscall(e_nl_lcd_type);
}

size_t real_puts(const char *ptr) {
	return wa_syscall1(e_puts, (uintptr_t)ptr);
}