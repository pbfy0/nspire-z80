#include <os.h>
#include "drz80.h"

#define INT_ON 1<<0
#define INT_HW1 1<<1
#define INT_HW2 1<<2
#define INT_LINK 1<<4
#define INT_CT1 1<<5
#define INT_CT2 1<<6
#define INT_CT3 1<<7

void int_mask_out(uint8_t val);
void int_ack_out(uint8_t val);
uint8_t int_id_in();
void int_fire(uint8_t num);
void int_callback();