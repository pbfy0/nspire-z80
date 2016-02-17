#include <stdint.h>
#pragma once
struct z80port {
	uint8_t number;
	struct z80port *mirror;
	union {
		void (*r)(uint8_t val);
		void (*n)(uint8_t val, uint8_t port);
	} out;
	
	union {
		uint8_t (*r)();
		uint8_t (*n)(uint8_t port);
	} in;
	
	uint8_t *ptr_val;
	uint8_t const_val;
	
	const char *name;
};

extern struct z80port ports[0x100];
void io_init();
