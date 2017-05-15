#ifndef C_SYSCALL_H
#define C_SYSCALL_H
#include <stdint.h>

static inline uint32_t wa_syscall(uint32_t nr)
{
	register uint32_t r0 asm("r0");
  
	asm volatile(
		"swi %[nr]\n"
		: "=r" (r0)
		: [nr] "i" (nr)
		: "memory", "r1", "r2", "r3", "r4", "r12", "lr");
  
	return r0;
}

static inline uint32_t wa_syscall1(uint32_t nr, uint32_t p1)
{
	register uint32_t r0 asm("r0") = p1;
  
	asm volatile(
		"swi %[nr]\n"
		: "=r" (r0)
		: [nr] "i" (nr), "r" (r0)
		: "memory", "r1", "r2", "r3", "r4", "r12", "lr");
  
	return r0;
}

static inline uint32_t wa_syscall2(uint32_t nr, uint32_t p1, uint32_t p2)
{
	register uint32_t r0 asm("r0") = p1;
	register uint32_t r1 asm("r1") = p2;
  
	asm volatile(
		"swi %[nr]\n"
		: "=r" (r0)
		: [nr] "i" (nr), "r" (r0), "r" (r1)
		: "memory", "r2", "r3", "r4", "r12", "lr");
  
	return r0;
}

static inline uint32_t wa_syscall3(uint32_t nr, uint32_t p1, uint32_t p2, uint32_t p3)
{
	register uint32_t r0 asm("r0") = p1;
	register uint32_t r1 asm("r1") = p2;
	register uint32_t r2 asm("r2") = p3;
	
	asm volatile(
		"swi %[nr]\n"
		: "=r" (r0)
		: [nr] "i" (nr), "r" (r0), "r" (r1), "r" (r2)
		: "memory", "r3", "r4", "r12", "lr");
  
	return r0;
}

static inline uint32_t wa_syscall4(uint32_t nr, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4)
{
	register uint32_t r0 asm("r0") = p1;
	register uint32_t r1 asm("r1") = p2;
	register uint32_t r2 asm("r2") = p3;
	register uint32_t r3 asm("r3") = p4;
 
	asm volatile(
		"swi %[nr]\n"
		: "=r" (r0)
		: [nr] "i" (nr), "r" (r0), "r" (r1), "r" (r2), "r" (r3)
		: "memory", "r4", "r12", "lr");
  
	return r0;
}

#endif