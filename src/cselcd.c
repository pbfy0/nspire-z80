#include <stdint.h>
#include "cselcd.h"
#include "cselcd_imp.h"
#include <stdio.h>

uint16_t port_idx = 0;
uint8_t setting_idx = 0;

uint8_t data_read_lo = 0;
uint8_t reading_data = 0;
uint8_t data_write_hi = 0;
uint8_t writing_data = 0;

void cselcd_data_set(uint16_t port, uint16_t val);
uint16_t cselcd_data_get(uint16_t port);

struct cselcd_port cselcd_ports[0xA0];

void cselcd_init() {
	printf("cselcd_init\n");
	int i;
	for(i = 0; i < 0xA0; i++){
		cselcd_ports[i] = (struct cselcd_port){ .pn = i };
	}
	cselcd_i_init();
	printf("cselcd_init end\n");
}
void cselcd_end() {
	cselcd_i_end();
}
void cselcd_ctrl_out(uint8_t val){
	//printf("cselcd_ctrl_out %02x\n", val);
	if(setting_idx){
		setting_idx = 0;
		port_idx |= val;
	}else{
		setting_idx = 1;
		port_idx = val << 8;
	}
	//printf("cselcd_ctrl_out end\n");
}

void cselcd_data_out(uint8_t val){
	//printf("cselcd_data_out\n");
	if(writing_data){
		writing_data = 0;
		unsigned v = (unsigned)data_write_hi << 8 | (unsigned)val;
		cselcd_data_set(port_idx & 0xff, v);
		//if(port_idx & 0xff != 0x22) printf("%04x -> p%04x\n", v, port_idx);
	}else{
		writing_data = 1;
		data_write_hi = val;
	}
	//printf("cselcd_data_out end\n");
}

uint8_t cselcd_data_in(){
	//printf("cselcd_data_in\n");
	if(reading_data) {
		reading_data = 0;
		return data_read_lo;
	}else{
		uint16_t t = cselcd_data_get(port_idx & 0xff);
		//if(port_idx & 0xff != 0x22) printf("%04x <- p%04x\n", t, port_idx);
		reading_data = 1;
		data_read_lo = t & 0xff;
		return t >> 8;
	}
	//printf("cselcd_data_in end\n");
}

void cselcd_data_set(uint16_t port, uint16_t val){
	//printf("cselcd_data_set %04x %04x\n", port, val);
	struct cselcd_port *p = &cselcd_ports[port];
	p->value = val;
	if(p->out) p->out(val);
	else if(p->ptr_value) *(p->ptr_value) = val;
}

uint16_t cselcd_data_get(uint16_t port){
	//printf("cselcd_data_get %04x\n", port);
	struct cselcd_port *p = &cselcd_ports[port];
	if(p->in) return p->in();
	if(p->ptr_value) return *(p->ptr_value);
	return p->value;
}