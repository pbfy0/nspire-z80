#include <os.h>
#define K(x) &(KEY_NSPIRE_##x)
uint8_t mon = 0;
t_key *n_key_ar[8][7] = {
	{K(DOWN),	K(ENTER),	K(NEGATIVE),	K(PERIOD),	K(0),	NULL,		K(EQU)},
	{K(LEFT),	K(PLUS),	K(3),			K(2),		K(1),	K(FLAG),	K(TRIG)},
	{K(RIGHT),	K(MINUS),	K(6),			K(5),		K(4),	K(EXP),		K(EE)},
	{K(UP),		K(MULTIPLY),K(9),			K(8),		K(7),	K(TENX),	K(FRAC)},
	{NULL,		K(DIVIDE),	K(LP),			K(RP),		K(COMMA),K(SQU),	K(EQU)},
	{NULL,		K(eEXP),	K(TAN),			K(COS),		K(SIN),	K(QUESEXCL),K(CTRL)},
	{NULL,		K(RET),		K(VAR),			K(SCRATCHPAD),K(DOC),K(MENU),	K(TAB)},
	{NULL,		NULL,		NULL,			K(PI),		NULL,	K(SHIFT),	K(DEL)}
};

void keypad_write(uint8_t val){
	//printf("keypad_write %02x\n", val);
	mon = ~val;
}

uint8_t keypad_read(){
	int i;
	uint8_t o;
	for(i = 0; i < 8; i++){
		if(mon & 1<<i){
			//t_key **k = n_key_ar[i];
			int j;
			for(j = 0; j < 7; j++){
				o |= isKeyPressed(*(n_key_ar[j][i])) << j;
			}
		}
	}
	//printf("keypad_read %02x\n", ~o);
	return ~o;
}