#include <stdint.h>
#pragma once
struct z80port {
	uint8_t number;
	void (*out)(uint8_t val);
	uint8_t (*in)();
	
	void (*n_out)(uint8_t port, uint8_t val);
	uint8_t (*n_in)(uint8_t port);
	
	uint8_t *ptr_val;
	uint8_t const_val;
	
	const char *name;
};

extern struct z80port ports[0x100];
void io_init();