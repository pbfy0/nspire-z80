#ifndef CALCTYPE_H
#define CALCTYPE_H

#define CALC_84P 0
#define CALC_84PSE 1
#define CALC_84PCSE 2

#ifndef CALC_TYPE
#define CALC_TYPE CALC_84PSE
#endif

#if CALC_TYPE == CALC_84PCSE
#define FLASH_SIZE 0x400000
#define BOOT_PAGE 0xff
#define EF_MASK 1

#elif CALC_TYPE == CALC_84P
#define FLASH_SIZE 0x100000
#define BOOT_PAGE 0x3f
#define EF_MASK 0

#elif CALC_TYPE == CALC_84PSE
#define FLASH_SIZE 0x200000
#define BOOT_PAGE 0x7f
#define EF_MASK 0

#endif

#define RAM_SIZE 0x20000
#endif