#ifndef CALCTYPE_H
#define CALCTYPE_H
#include <stdbool.h>

struct calc_type {
	unsigned flash_size;
	unsigned i_flash_size;
	unsigned boot_page;
	unsigned ef_mask;
	bool cselcd;
};
extern struct calc_type g_calc;

#define FLASH_SIZE g_calc.flash_size
#define BOOT_PAGE g_calc.boot_page
#define EF_MASK g_calc.ef_mask

#define RAM_SIZE 0x20000
#endif
