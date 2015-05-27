#include <stdint.h>
uint8_t mem_size = 3 | 2<<4;
uint8_t mem_size_in(){
	return mem_size;
}

void mem_size_out(uint8_t v){
	mem_size = v;
}