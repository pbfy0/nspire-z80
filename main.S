	.cpu arm926ej-s
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 18, 4
	.file	"main.c"
	.bss
	.align	2
_syscallvar_savedlr:
	.space	4
	.text
	.align	2
	.type	string_sprintf_utf16, %function
string_sprintf_utf16:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 4, pretend = 12, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 1
@ 251 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 226
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	string_sprintf_utf16, .-string_sprintf_utf16
	.align	2
	.type	_show_msgbox_2b_swi, %function
_show_msgbox_2b_swi:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 259 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 93
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	_show_msgbox_2b_swi, .-_show_msgbox_2b_swi
	.align	2
	.type	_show_msgbox_3b_swi, %function
_show_msgbox_3b_swi:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 260 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 94
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	_show_msgbox_3b_swi, .-_show_msgbox_3b_swi
	.align	2
	.type	_show_1NumericInput_swi, %function
_show_1NumericInput_swi:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 262 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 203
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	_show_1NumericInput_swi, .-_show_1NumericInput_swi
	.align	2
	.type	_show_2NumericInput_swi, %function
_show_2NumericInput_swi:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 264 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 204
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	_show_2NumericInput_swi, .-_show_2NumericInput_swi
	.align	2
	.type	_show_msgUserInput_swi, %function
_show_msgUserInput_swi:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 266 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 205
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	_show_msgUserInput_swi, .-_show_msgUserInput_swi
	.align	2
	.type	memset, %function
memset:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	str	r0, [fp, #-8]
	str	r1, [fp, #-12]
	str	r2, [fp, #-16]
	ldr	r3, [fp, #-8]
	mov	r0, r3
	ldr	r3, [fp, #-12]
	mov	r1, r3
	ldr	r2, [fp, #-16]
@ 294 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 swi 7
@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	memset, .-memset
	.align	2
	.type	printf, %function
printf:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 4, pretend = 16, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 1
@ 320 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 10
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	printf, .-printf
	.align	2
	.type	sprintf, %function
sprintf:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 4, pretend = 12, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 1
@ 321 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 11
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	sprintf, .-sprintf
	.align	2
	.type	fprintf, %function
fprintf:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 4, pretend = 12, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 1
@ 322 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 12
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	fprintf, .-fprintf
	.align	2
	.type	sscanf, %function
sscanf:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 4, pretend = 12, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 1
@ 323 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 266
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	sscanf, .-sscanf
	.align	2
	.type	snprintf, %function
snprintf:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 4, pretend = 8, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 1
@ 330 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 332
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	snprintf, .-snprintf
	.align	2
	.type	fopen, %function
fopen:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	str	r1, [fp, #-12]
	ldr	r3, [fp, #-8]
	mov	r0, r3
	ldr	r3, [fp, #-12]
	mov	r1, r3
@ 353 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 swi 0
@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	fopen, .-fopen
	.align	2
	.type	fread, %function
fread:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	str	r0, [fp, #-8]
	str	r1, [fp, #-12]
	str	r2, [fp, #-16]
	str	r3, [fp, #-20]
	ldr	r3, [fp, #-8]
	mov	r0, r3
	ldr	r1, [fp, #-12]
	ldr	r2, [fp, #-16]
	ldr	r3, [fp, #-20]
@ 355 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 swi 1
@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	fread, .-fread
	.align	2
	.type	fclose, %function
fclose:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	mov	r0, r3
@ 358 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 swi 3
@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	fclose, .-fclose
	.align	2
	.type	fseek, %function
fseek:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	str	r0, [fp, #-8]
	str	r1, [fp, #-12]
	str	r2, [fp, #-16]
	ldr	r3, [fp, #-8]
	mov	r0, r3
	ldr	r3, [fp, #-12]
	mov	r1, r3
	ldr	r3, [fp, #-16]
	mov	r2, r3
@ 360 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 swi 33
@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	fseek, .-fseek
	.align	2
	.type	ftell, %function
ftell:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	mov	r0, r3
@ 361 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 swi 89
@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	ftell, .-ftell
	.align	2
	.type	deflateInit2__swi, %function
deflateInit2__swi:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 496 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 82
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	deflateInit2__swi, .-deflateInit2__swi
	.align	2
	.type	luaL_error, %function
luaL_error:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 4, pretend = 12, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 1
@ 507 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 100
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	luaL_error, .-luaL_error
	.align	2
	.type	lua_pushfstring, %function
lua_pushfstring:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 4, pretend = 12, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 1
@ 590 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 168
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	lua_pushfstring, .-lua_pushfstring
	.align	2
	.type	usb_register_driver, %function
usb_register_driver:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 627 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 258
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	usb_register_driver, .-usb_register_driver
	.align	2
	.type	usbd_setup_xfer, %function
usbd_setup_xfer:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 16, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 641 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 232
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	.size	usbd_setup_xfer, .-usbd_setup_xfer
	.align	2
	.type	usbd_setup_isoc_xfer, %function
usbd_setup_isoc_xfer:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 12, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 643 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 233
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	.size	usbd_setup_isoc_xfer, .-usbd_setup_isoc_xfer
	.align	2
	.type	usbd_get_xfer_status, %function
usbd_get_xfer_status:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 644 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 234
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	.size	usbd_get_xfer_status, .-usbd_get_xfer_status
	.align	2
	.type	usbd_open_pipe_intr, %function
usbd_open_pipe_intr:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 20, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 664 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 244
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	and	r3, r3, #255
	mov	r0, r3
	.size	usbd_open_pipe_intr, .-usbd_open_pipe_intr
	.align	2
	.type	usbd_do_request_flags, %function
usbd_do_request_flags:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 667 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 246
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	and	r3, r3, #255
	mov	r0, r3
	.size	usbd_do_request_flags, .-usbd_do_request_flags
	.align	2
	.type	usbd_do_request_flags_pipe, %function
usbd_do_request_flags_pipe:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 8, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 668 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 247
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	and	r3, r3, #255
	mov	r0, r3
	.size	usbd_do_request_flags_pipe, .-usbd_do_request_flags_pipe
	.align	2
	.type	TI_NN_Read, %function
TI_NN_Read:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 698 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 272
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r3, r3, asl #16
	mov	r3, r3, lsr #16
	mov	r3, r3, asl #16
	mov	r3, r3, asr #16
	mov	r0, r3
	.size	TI_NN_Read, .-TI_NN_Read
	.align	2
	.type	read_nand, %function
read_nand:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 8, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 716 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 336
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	.size	read_nand, .-read_nand
	.align	2
	.type	exit, %function
exit:
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	ldr	r3, .L31
.LPIC0:
	add	r3, pc, r3
	ldr	r2, .L31+4
	ldr	r2, [r3, r2]
	ldr	r2, [r2, #0]
	ldr	r1, .L31+8
	ldr	r3, [r3, r1]
@ 748 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 mov sp, r2 
 mov pc, r3
@ 0 "" 2
.L32:
	.align	2
.L31:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC0+8)
	.word	__crt0_savedsp(GOT)
	.word	__crt0exit(GOT)
	.size	exit, .-exit
	.align	2
	.type	abort, %function
abort:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	mvn	r0, #0
	bl	exit(PLT)
	.size	abort, .-abort
	.comm	ZCpu,116,4
	.comm	flag,1,1
	.section	.rodata
	.align	2
.LC1:
	.ascii	"rom.tns\000"
	.align	2
.LC2:
	.ascii	"rb\000"
	.align	2
.LC3:
	.ascii	"Error\000"
	.align	2
.LC4:
	.ascii	"Could not open rom\000"
	.align	2
.LC5:
	.ascii	"%08x\012\000"
	.align	2
.LC0:
	.word	28
	.word	128
	.word	28
	.word	128
	.byte	0
	.space	3
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 144
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, fp, lr}
	add	fp, sp, #16
	sub	sp, sp, #148
	ldr	r4, .L48
.LPIC5:
	add	r4, pc, r4
	ldr	r3, .L48+4
.LPIC1:
	add	r3, pc, r3
	mov	r0, r3
	ldr	r3, .L48+8
.LPIC2:
	add	r3, pc, r3
	mov	r1, r3
	bl	fopen(PLT)
	str	r0, [fp, #-28]
	ldr	r3, [fp, #-28]
	cmp	r3, #0
	bne	.L35
	ldr	r3, .L48+12
.LPIC3:
	add	r3, pc, r3
	mov	r0, r3
	ldr	r3, .L48+16
.LPIC4:
	add	r3, pc, r3
	mov	r1, r3
	mov	r2, #0
	bl	_show_msgbox(PLT)
	mov	r3, #1
	b	.L36
.L35:
	ldr	r0, [fp, #-28]
	mov	r1, #0
	mov	r2, #2
	bl	fseek(PLT)
	ldr	r0, [fp, #-28]
	bl	ftell(PLT)
	str	r0, [fp, #-32]
	ldr	r0, [fp, #-28]
	mov	r1, #0
	mov	r2, #0
	bl	fseek(PLT)
	bl	mmap_init(PLT)
	ldr	r3, .L48+20
	ldr	r3, [r4, r3]
	ldr	r2, [r3, #0]
	ldr	r3, [fp, #-32]
	mov	r0, r2
	mov	r1, #1
	mov	r2, r3
	ldr	r3, [fp, #-28]
	bl	fread(PLT)
	ldr	r0, [fp, #-28]
	bl	fclose(PLT)
	bl	lcd_init(PLT)
	bl	cpu_init(PLT)
	bl	io_init(PLT)
	ldr	r3, .L48+24
	ldr	r3, [r4, r3]
	ldr	r2, [r3, #28]
	ldr	r3, .L48+20
	ldr	r3, [r4, r3]
	ldr	r3, [r3, #0]
	rsb	r2, r3, r2
	ldr	r3, .L48+28
.LPIC6:
	add	r3, pc, r3
	mov	r0, r3
	mov	r1, r2
	bl	printf(PLT)
	ldr	r3, .L48+24
	ldr	r3, [r4, r3]
	ldr	r2, [r3, #0]
	ldr	r3, .L48+20
	ldr	r3, [r4, r3]
	ldr	r3, [r3, #0]
	rsb	r2, r3, r2
	ldr	r3, .L48+32
.LPIC7:
	add	r3, pc, r3
	mov	r0, r3
	mov	r1, r2
	bl	printf(PLT)
	bl	interrupt_init(PLT)
	bl	next_timer(PLT)
	str	r0, [fp, #-24]
	mov	r3, #0
	str	r3, [fp, #-36]
	b	.L44
.L47:
	mov	r0, r0	@ nop
.L44:
	ldr	r3, [fp, #-24]
	ldr	r2, .L48+24
	ldr	r2, [r4, r2]
	mov	r0, r2
	mov	r1, r3
	bl	DrZ80Run(PLT)
	str	r0, [fp, #-40]
	ldr	r3, .L48+24
	ldr	r3, [r4, r3]
	ldr	r3, [r3, #0]
	str	r3, [fp, #-44]
	ldr	r0, [fp, #-40]
	bl	timer_after(PLT)
	str	r0, [fp, #-24]
	ldr	r3, .L48+36
.LPIC8:
	add	r3, pc, r3
	sub	ip, fp, #164
	mov	lr, r3
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldr	r3, [lr, #0]
	str	r3, [ip, #0]
	ldrb	r3, [fp, #-148]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L37
	bl	_is_touchpad(PLT)
	mov	r3, r0
	cmp	r3, #0
	beq	.L37
	ldr	r3, .L48+40
.LPIC9:
	add	r3, pc, r3
	sub	ip, fp, #144
	mov	lr, r3
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldr	r3, [lr, #0]
	str	r3, [ip, #0]
	ldrb	r3, [fp, #-128]	@ zero_extendqisi2
	mov	r0, r3
	bl	touchpad_arrow_pressed(PLT)
	mov	r3, r0
	cmp	r3, #0
	moveq	r3, #0
	movne	r3, #1
	and	r3, r3, #255
	b	.L38
.L37:
	bl	hwtype(PLT)
	mov	r3, r0
	cmp	r3, #0
	moveq	r5, #0
	movne	r5, #1
	bl	_is_touchpad(PLT)
	mov	r3, r0
	cmp	r3, #0
	beq	.L39
	ldr	r3, .L48+44
.LPIC10:
	add	r3, pc, r3
	sub	ip, fp, #124
	mov	lr, r3
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldr	r3, [lr, #0]
	str	r3, [ip, #0]
	ldr	r3, [fp, #-116]
	add	r3, r3, #-1879048192
	add	r3, r3, #917504
	ldrh	r3, [r3, #0]	@ movhi
	mov	r3, r3, asl #16
	mov	r3, r3, lsr #16
	mov	r3, r3, asl #16
	mov	r6, r3, asr #16
	ldr	r3, .L48+48
.LPIC11:
	add	r3, pc, r3
	sub	ip, fp, #104
	mov	lr, r3
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldr	r3, [lr, #0]
	str	r3, [ip, #0]
	ldr	r3, [fp, #-92]
	and	r3, r6, r3
	cmp	r3, #0
	movne	r3, #0
	moveq	r3, #1
	b	.L40
.L39:
	ldr	r3, .L48+52
.LPIC12:
	add	r3, pc, r3
	sub	ip, fp, #84
	mov	lr, r3
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldr	r3, [lr, #0]
	str	r3, [ip, #0]
	ldr	r3, [fp, #-84]
	add	r3, r3, #-1879048192
	add	r3, r3, #917504
	ldrh	r3, [r3, #0]	@ movhi
	mov	r3, r3, asl #16
	mov	r3, r3, lsr #16
	mov	r3, r3, asl #16
	mov	r6, r3, asr #16
	ldr	r3, .L48+56
.LPIC13:
	add	r3, pc, r3
	sub	ip, fp, #64
	mov	lr, r3
	ldmia	lr!, {r0, r1, r2, r3}
	stmia	ip!, {r0, r1, r2, r3}
	ldr	r3, [lr, #0]
	str	r3, [ip, #0]
	ldr	r3, [fp, #-60]
	and	r3, r6, r3
	cmp	r3, #0
	movne	r3, #0
	moveq	r3, #1
.L40:
	cmp	r5, r3
	moveq	r3, #0
	movne	r3, #1
	and	r3, r3, #255
.L38:
	cmp	r3, #0
	bne	.L46
.L41:
	ldr	r3, .L48+60
	ldr	r3, [r4, r3]
	ldrb	r3, [r3, #0]
	and	r3, r3, #255
	cmp	r3, #0
	beq	.L47
	ldr	r3, .L48+60
	ldr	r3, [r4, r3]
	mov	r2, #0
	strb	r2, [r3, #0]
	b	.L47
.L46:
	mov	r0, r0	@ nop
.L45:
	bl	lcd_end(PLT)
	bl	interrupt_end(PLT)
	bl	mmap_end(PLT)
	mov	r3, #0
.L36:
	mov	r0, r3
	sub	sp, fp, #16
	ldmfd	sp!, {r4, r5, r6, fp, pc}
.L49:
	.align	2
.L48:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC5+8)
	.word	.LC1-(.LPIC1+8)
	.word	.LC2-(.LPIC2+8)
	.word	.LC3-(.LPIC3+8)
	.word	.LC4-(.LPIC4+8)
	.word	flash(GOT)
	.word	ZCpu(GOT)
	.word	.LC5-(.LPIC6+8)
	.word	.LC5-(.LPIC7+8)
	.word	.LC0-(.LPIC8+8)
	.word	.LC0-(.LPIC9+8)
	.word	.LC0-(.LPIC10+8)
	.word	.LC0-(.LPIC11+8)
	.word	.LC0-(.LPIC12+8)
	.word	.LC0-(.LPIC13+8)
	.word	flag(GOT)
	.size	main, .-main
	.align	2
	.global	cpu_init
	.type	cpu_init, %function
cpu_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {r3, r4, fp, lr}
	add	fp, sp, #12
	ldr	r4, .L51
.LPIC14:
	add	r4, pc, r4
	ldr	r3, .L51+4
	ldr	r3, [r4, r3]
	mov	r0, r3
	mov	r1, #0
	mov	r2, #116
	bl	memset(PLT)
	ldr	r3, .L51+4
	ldr	r3, [r4, r3]
	ldr	r2, .L51+8
	ldr	r2, [r4, r2]
	str	r2, [r3, #108]
	ldr	r3, .L51+4
	ldr	r3, [r4, r3]
	ldr	r2, .L51+12
	ldr	r2, [r4, r2]
	str	r2, [r3, #104]
	ldr	r3, .L51+4
	ldr	r3, [r4, r3]
	ldr	r2, .L51+16
	ldr	r2, [r4, r2]
	str	r2, [r3, #96]
	ldr	r3, .L51+4
	ldr	r3, [r4, r3]
	ldr	r2, .L51+20
	ldr	r2, [r4, r2]
	str	r2, [r3, #100]
	ldr	r3, .L51+4
	ldr	r3, [r4, r3]
	ldr	r2, .L51+24
	ldr	r2, [r4, r2]
	str	r2, [r3, #80]
	ldr	r3, .L51+4
	ldr	r3, [r4, r3]
	ldr	r2, .L51+28
	ldr	r2, [r4, r2]
	str	r2, [r3, #84]
	ldr	r3, .L51+4
	ldr	r3, [r4, r3]
	ldr	r2, .L51+32
	ldr	r2, [r4, r2]
	str	r2, [r3, #88]
	ldr	r3, .L51+4
	ldr	r3, [r4, r3]
	ldr	r2, .L51+36
	ldr	r2, [r4, r2]
	str	r2, [r3, #92]
	ldr	r3, .L51+40
	ldr	r3, [r4, r3]
	mov	r2, r3
	ldr	r3, .L51+4
	ldr	r3, [r4, r3]
	str	r2, [r3, #76]
	mov	r0, #0
	bl	cpu_rebasePC(PLT)
	mov	r2, r0
	ldr	r3, .L51+4
	ldr	r3, [r4, r3]
	str	r2, [r3, #0]
	ldr	r3, .L51+4
	ldr	r3, [r4, r3]
	ldr	r2, .L51+44
	str	r2, [r3, #24]
	ldmfd	sp!, {r3, r4, fp, pc}
.L52:
	.align	2
.L51:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC14+8)
	.word	ZCpu(GOT)
	.word	cpu_rebasePC(GOT)
	.word	cpu_rebaseSP(GOT)
	.word	cpu_read8(GOT)
	.word	cpu_read16(GOT)
	.word	cpu_write8(GOT)
	.word	cpu_write16(GOT)
	.word	cpu_in(GOT)
	.word	cpu_out(GOT)
	.word	cpu_irq_callback(GOT)
	.word	65535
	.size	cpu_init, .-cpu_init
	.section	.rodata
	.align	2
.LC6:
	.ascii	"irq\012\000"
	.text
	.align	2
	.global	cpu_irq_callback
	.type	cpu_irq_callback, %function
cpu_irq_callback:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	ldr	r3, .L54
.LPIC15:
	add	r3, pc, r3
	mov	r0, r3
	bl	printf(PLT)
	bl	int_callback(PLT)
	ldmfd	sp!, {fp, pc}
.L55:
	.align	2
.L54:
	.word	.LC6-(.LPIC15+8)
	.size	cpu_irq_callback, .-cpu_irq_callback
	.section	.rodata
	.align	2
.LC7:
	.ascii	"pc(a) %02x v %04x\012\000"
	.text
	.align	2
	.global	pdb
	.type	pdb, %function
pdb:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	mov	r3, r3, asl #16
	mov	r3, r3, lsr #16
	mov	r0, r3
	bl	cpu_read16(PLT)
	mov	r3, r0
	mov	r2, r3
	ldr	r3, .L57
.LPIC16:
	add	r3, pc, r3
	mov	r0, r3
	ldr	r1, [fp, #-8]
	bl	printf(PLT)
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
.L58:
	.align	2
.L57:
	.word	.LC7-(.LPIC16+8)
	.size	pdb, .-pdb
	.align	2
	.global	cpu_rebasePC
	.type	cpu_rebasePC, %function
cpu_rebasePC:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {r4, fp, lr}
	add	fp, sp, #8
	sub	sp, sp, #12
	mov	r3, r0
	strh	r3, [fp, #-14]	@ movhi
	ldr	r4, .L60
.LPIC17:
	add	r4, pc, r4
	ldrh	r3, [fp, #-14]
	mov	r0, r3
	bl	mmap_check_endboot(PLT)
	ldrh	r3, [fp, #-14]
	mov	r0, r3
	bl	mmap_base_addr(PLT)
	mov	r3, r0
	mov	r2, r3
	ldr	r3, .L60+4
	ldr	r3, [r4, r3]
	str	r2, [r3, #28]
	ldr	r3, .L60+4
	ldr	r3, [r4, r3]
	ldr	r2, [r3, #28]
	ldrh	r3, [fp, #-14]
	add	r3, r2, r3
	mov	r0, r3
	sub	sp, fp, #8
	ldmfd	sp!, {r4, fp, pc}
.L61:
	.align	2
.L60:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC17+8)
	.word	ZCpu(GOT)
	.size	cpu_rebasePC, .-cpu_rebasePC
	.section	.rodata
	.align	2
.LC8:
	.ascii	"rebaseSP 0x%x\012\000"
	.text
	.align	2
	.global	cpu_rebaseSP
	.type	cpu_rebaseSP, %function
cpu_rebaseSP:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {r4, fp, lr}
	add	fp, sp, #8
	sub	sp, sp, #12
	mov	r3, r0
	strh	r3, [fp, #-14]	@ movhi
	ldr	r4, .L63
.LPIC19:
	add	r4, pc, r4
	ldrh	r2, [fp, #-14]
	ldr	r3, .L63+4
.LPIC18:
	add	r3, pc, r3
	mov	r0, r3
	mov	r1, r2
	bl	printf(PLT)
	ldrh	r3, [fp, #-14]
	mov	r0, r3
	bl	mmap_base_addr(PLT)
	mov	r3, r0
	mov	r2, r3
	ldr	r3, .L63+8
	ldr	r3, [r4, r3]
	str	r2, [r3, #32]
	ldr	r3, .L63+8
	ldr	r3, [r4, r3]
	ldr	r2, [r3, #32]
	ldrh	r3, [fp, #-14]
	add	r3, r2, r3
	mov	r0, r3
	sub	sp, fp, #8
	ldmfd	sp!, {r4, fp, pc}
.L64:
	.align	2
.L63:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC19+8)
	.word	.LC8-(.LPIC18+8)
	.word	ZCpu(GOT)
	.size	cpu_rebaseSP, .-cpu_rebaseSP
	.align	2
	.global	cpu_read16
	.type	cpu_read16, %function
cpu_read16:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	mov	r3, r0
	strh	r3, [fp, #-14]	@ movhi
	ldrh	r3, [fp, #-14]
	mov	r0, r3
	bl	mmap_z80_to_arm(PLT)
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	mov	r2, r3
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	mov	r3, r3, asl #8
	mov	r3, r3, asl #16
	mov	r3, r3, lsr #16
	orr	r3, r2, r3
	mov	r3, r3, asl #16
	mov	r3, r3, lsr #16
	mov	r3, r3, asl #16
	mov	r3, r3, lsr #16
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	cpu_read16, .-cpu_read16
	.align	2
	.global	cpu_read8
	.type	cpu_read8, %function
cpu_read8:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	mov	r3, r0
	strh	r3, [fp, #-6]	@ movhi
	ldrh	r3, [fp, #-6]
	mov	r0, r3
	bl	mmap_z80_to_arm(PLT)
	mov	r3, r0
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	cpu_read8, .-cpu_read8
	.section	.rodata
	.align	2
.LC9:
	.ascii	"Invalid memory access: %p %04x\000"
	.text
	.align	2
	.global	cpu_write16
	.type	cpu_write16, %function
cpu_write16:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {r4, fp, lr}
	add	fp, sp, #8
	sub	sp, sp, #20
	mov	r2, r0
	mov	r3, r1
	strh	r2, [fp, #-22]	@ movhi
	strh	r3, [fp, #-24]	@ movhi
	ldr	r4, .L71
.LPIC20:
	add	r4, pc, r4
	ldrh	r3, [fp, #-24]
	mov	r0, r3
	bl	mmap_z80_to_arm(PLT)
	str	r0, [fp, #-16]
	ldr	r3, .L71+4
	ldr	r3, [r4, r3]
	ldr	r3, [r3, #0]
	ldr	r2, [fp, #-16]
	cmp	r2, r3
	bcc	.L68
	ldr	r3, .L71+4
	ldr	r3, [r4, r3]
	ldr	r3, [r3, #0]
	add	r2, r3, #2097152
	ldr	r3, [fp, #-16]
	cmp	r2, r3
	bhi	.L69
.L68:
	ldr	r3, .L71+8
	ldr	r3, [r4, r3]
	ldr	r3, [r3, #0]
	ldr	r2, [fp, #-16]
	cmp	r2, r3
	bcc	.L70
	ldr	r3, .L71+8
	ldr	r3, [r4, r3]
	ldr	r3, [r3, #0]
	add	r2, r3, #131072
	ldr	r3, [fp, #-16]
	cmp	r2, r3
	bhi	.L69
.L70:
	ldrh	r2, [fp, #-24]
	ldr	r3, .L71+12
.LPIC21:
	add	r3, pc, r3
	mov	r0, r3
	ldr	r1, [fp, #-16]
	bl	printf(PLT)
.L69:
	ldrh	r3, [fp, #-22]	@ movhi
	and	r2, r3, #255
	ldr	r3, [fp, #-16]
	strb	r2, [r3, #0]
	ldr	r3, [fp, #-16]
	add	r3, r3, #1
	ldrh	r2, [fp, #-22]
	mov	r2, r2, lsr #8
	mov	r2, r2, asl #16
	mov	r2, r2, lsr #16
	and	r2, r2, #255
	strb	r2, [r3, #0]
	sub	sp, fp, #8
	ldmfd	sp!, {r4, fp, pc}
.L72:
	.align	2
.L71:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC20+8)
	.word	flash(GOT)
	.word	ram(GOT)
	.word	.LC9-(.LPIC21+8)
	.size	cpu_write16, .-cpu_write16
	.align	2
	.global	cpu_write8
	.type	cpu_write8, %function
cpu_write8:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {r4, fp, lr}
	add	fp, sp, #8
	sub	sp, sp, #20
	mov	r2, r0
	mov	r3, r1
	strb	r2, [fp, #-21]
	strh	r3, [fp, #-24]	@ movhi
	ldr	r4, .L77
.LPIC22:
	add	r4, pc, r4
	ldrh	r3, [fp, #-24]
	mov	r0, r3
	bl	mmap_z80_to_arm(PLT)
	str	r0, [fp, #-16]
	ldr	r3, .L77+4
	ldr	r3, [r4, r3]
	ldr	r3, [r3, #0]
	ldr	r2, [fp, #-16]
	cmp	r2, r3
	bcc	.L74
	ldr	r3, .L77+4
	ldr	r3, [r4, r3]
	ldr	r3, [r3, #0]
	add	r2, r3, #2097152
	ldr	r3, [fp, #-16]
	cmp	r2, r3
	bhi	.L75
.L74:
	ldr	r3, .L77+8
	ldr	r3, [r4, r3]
	ldr	r3, [r3, #0]
	ldr	r2, [fp, #-16]
	cmp	r2, r3
	bcc	.L76
	ldr	r3, .L77+8
	ldr	r3, [r4, r3]
	ldr	r3, [r3, #0]
	add	r2, r3, #131072
	ldr	r3, [fp, #-16]
	cmp	r2, r3
	bhi	.L75
.L76:
	ldrh	r2, [fp, #-24]
	ldr	r3, .L77+12
.LPIC23:
	add	r3, pc, r3
	mov	r0, r3
	ldr	r1, [fp, #-16]
	bl	printf(PLT)
.L75:
	ldr	r3, [fp, #-16]
	ldrb	r2, [fp, #-21]
	strb	r2, [r3, #0]
	sub	sp, fp, #8
	ldmfd	sp!, {r4, fp, pc}
.L78:
	.align	2
.L77:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC22+8)
	.word	flash(GOT)
	.word	ram(GOT)
	.word	.LC9-(.LPIC23+8)
	.size	cpu_write8, .-cpu_write8
	.section	.rodata
	.align	2
.LC10:
	.ascii	"Read %02x from port %02x (%s)\012\000"
	.text
	.align	2
	.global	cpu_in
	.type	cpu_in, %function
cpu_in:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	mov	r3, r0
	strh	r3, [fp, #-14]	@ movhi
	ldr	r3, .L80
.LPIC24:
	add	r3, pc, r3
	ldrh	r2, [fp, #-14]	@ movhi
	and	r2, r2, #255
	mov	r2, r2, asl #5
	ldr	r1, .L80+4
	ldr	r3, [r3, r1]
	add	r3, r2, r3
	str	r3, [fp, #-8]
	ldr	r0, [fp, #-8]
	bl	port_get(PLT)
	mov	r3, r0
	strb	r3, [fp, #-9]
	ldrb	r1, [fp, #-9]	@ zero_extendqisi2
	ldr	r3, [fp, #-8]
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	mov	r2, r3
	ldr	r3, [fp, #-8]
	ldr	ip, [r3, #28]
	ldr	r3, .L80+8
.LPIC25:
	add	r3, pc, r3
	mov	r0, r3
	mov	r3, ip
	bl	printf(PLT)
	ldrb	r3, [fp, #-9]	@ zero_extendqisi2
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
.L81:
	.align	2
.L80:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC24+8)
	.word	ports(GOT)
	.word	.LC10-(.LPIC25+8)
	.size	cpu_in, .-cpu_in
	.section	.rodata
	.align	2
.LC11:
	.ascii	"Wrote %02x to port %02x (%s)\012\000"
	.text
	.align	2
	.global	cpu_out
	.type	cpu_out, %function
cpu_out:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	mov	r2, r0
	mov	r3, r1
	strh	r2, [fp, #-14]	@ movhi
	strb	r3, [fp, #-15]
	ldr	r3, .L83
.LPIC26:
	add	r3, pc, r3
	ldrh	r2, [fp, #-14]	@ movhi
	and	r2, r2, #255
	mov	r1, r2, asl #5
	ldr	r2, .L83+4
	ldr	r2, [r3, r2]
	add	r2, r1, r2
	str	r2, [fp, #-8]
@ 179 "main.c" 1
	 mov r0, r6
@ 0 "" 2
	mov	r2, r0
	ldr	r1, .L83+8
	ldr	r3, [r3, r1]
	ldr	r3, [r3, #28]
	rsb	r3, r3, #0
	add	r3, r2, r3
	strh	r3, [fp, #-10]	@ movhi
	ldrb	r3, [fp, #-15]	@ zero_extendqisi2
	ldr	r0, [fp, #-8]
	mov	r1, r3
	bl	port_set(PLT)
	ldrb	r1, [fp, #-15]	@ zero_extendqisi2
	ldr	r3, [fp, #-8]
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	mov	r2, r3
	ldr	r3, [fp, #-8]
	ldr	ip, [r3, #28]
	ldr	r3, .L83+12
.LPIC27:
	add	r3, pc, r3
	mov	r0, r3
	mov	r3, ip
	bl	printf(PLT)
	ldrh	r3, [fp, #-10]
	mov	r0, r3
	bl	cpu_rebasePC(PLT)
	mov	r3, r0
	mov	r0, r3
@ 184 "main.c" 1
	 mov r6, r0
@ 0 "" 2
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
.L84:
	.align	2
.L83:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC26+8)
	.word	ports(GOT)
	.word	ZCpu(GOT)
	.word	.LC11-(.LPIC27+8)
	.size	cpu_out, .-cpu_out
	.align	2
	.global	port_get
	.type	port_get, %function
port_get:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #8]
	cmp	r3, #0
	beq	.L86
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #8]
	blx	r3
	mov	r3, r0
	b	.L87
.L86:
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #16]
	cmp	r3, #0
	beq	.L88
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #16]
	ldr	r2, [fp, #-8]
	ldrb	r2, [r2, #0]	@ zero_extendqisi2
	mov	r0, r2
	blx	r3
	mov	r3, r0
	b	.L87
.L88:
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq	.L89
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #20]
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	b	.L87
.L89:
	ldr	r3, [fp, #-8]
	ldrb	r3, [r3, #24]	@ zero_extendqisi2
.L87:
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	port_get, .-port_get
	.align	2
	.global	port_set
	.type	port_set, %function
port_set:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	mov	r3, r1
	strb	r3, [fp, #-9]
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L91
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #4]
	ldrb	r2, [fp, #-9]	@ zero_extendqisi2
	mov	r0, r2
	blx	r3
	b	.L90
.L91:
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #12]
	cmp	r3, #0
	beq	.L93
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #12]
	ldr	r2, [fp, #-8]
	ldrb	r1, [r2, #0]	@ zero_extendqisi2
	ldrb	r2, [fp, #-9]	@ zero_extendqisi2
	mov	r0, r1
	mov	r1, r2
	blx	r3
	b	.L90
.L93:
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq	.L90
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #20]
	ldrb	r2, [fp, #-9]
	strb	r2, [r3, #0]
.L90:
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	port_set, .-port_set
	.ident	"GCC: (GNU) 4.6.2"
