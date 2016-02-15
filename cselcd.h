#pragma once
#include <stdint.h>

void cselcd_ctrl_out(uint8_t val);
void cselcd_data_out(uint8_t val);
uint8_t cselcd_data_in();

void cselcd_init();
void cselcd_end();

struct cselcd_port {
	void (*out)(uint16_t val);
	uint16_t (*in)();
	uint16_t *ptr_value;
	uint16_t pn;
	uint16_t value;
};

extern struct cselcd_port cselcd_ports[0xA0];