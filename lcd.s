	.cpu arm926ej-s
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 2
	.eabi_attribute 18, 4
	.file	"lcd.c"
	
.align 4
_n_set_84_pixel:
	push {r4, lr}
	mov r4, #320
	mla r3, r1, r4, r3
	add r3, r0
	adr r4, px_offsets
	mov r0, #18
1:	subs r0, #2
	ldrh r1, [r4, r0]
	strb r2, [r3, r1]
	bne 1b
	pop {r4, pc}

	.text
	.align	2
	.global	lcd_init
	.type	lcd_init, %function
lcd_init:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, lr}
	ldr	r7, .L5
	sub	sp, sp, #20
.LPIC0:
	add	r7, pc, r7
	mov	r0, #76800
@ 290 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 swi 5
@ 0 "" 2
	ldr	r3, .L5+4
	mov	r1, #255
	ldr	r5, [r7, r3]
	mov	r2, #76800
	str	r0, [r5, #0]
@ 294 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 swi 7
@ 0 "" 2
	ldr	r4, .L5+8
	ldr	r6, .L5+12
.LPIC1:
	add	r4, pc, r4
	ldmia	r4, {r0, r1}
	stmia	sp, {r0, r1}
	bl	hwtype(PLT)
	add	r1, sp, #16
	add	ip, sp, #16
	add	r0, r1, r0, asl #2
	ldr	r3, [r0, #-16]
	ldmia	r4, {r0, r1}
	ldr	r2, [r3, #0]
	stmdb	ip, {r0, r1}
	ldr	r4, .L5+16
	bic	r2, r2, #14
	str	r2, [r3, #0]
	bl	hwtype(PLT)
	add	r1, sp, #16
	ldr	r2, [r5, #0]
	mov	r3, #-1073741824
	add	r0, r1, r0, asl #2
	ldr	r1, [r0, #-8]
	ldr	r0, [r1, #0]
	orr	r0, r0, #6
	str	r0, [r1, #0]
	ldr	r1, .L5+20
	ldr	r0, [r3, #16]
	ldr	r1, [r7, r1]
	str	r2, [r3, #16]
	ldr	r3, .L5+24
	str	r0, [r1, #0]
	ldr	r3, [r7, r3]
	ldr	r2, .L5+28
	ldr	r3, [r3, #0]
	str	r2, [r3, #0]
	ldr	r2, .L5+32
	str	r2, [r3, #508]
.L2:
	ldr	r0, [r5, #0]
	mov	r1, #0
	add	r0, r0, r4
	mov	r2, #288
@ 294 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 swi 7
@ 0 "" 2
	add	r4, r4, #320
	cmp	r4, r6
	bne	.L2
	add	sp, sp, #20
	ldmfd	sp!, {r4, r5, r6, r7, pc}
.L6:
	.align	2
.L5:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC0+8)
	.word	framebuffer(GOT)
	.word	.LANCHOR0-(.LPIC1+8)
	.word	69136
	.word	7696
	.word	bfb(GOT)
	.word	palette(GOT)
	.word	65535
	.word	1454702592
	.size	lcd_init, .-lcd_init
	.align	2
	.global	lcd_end
	.type	lcd_end, %function
lcd_end:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, lr}
	ldr	r4, .L8
	ldr	r3, .L8+4
.LPIC3:
	add	r4, pc, r4
	ldr	r3, [r4, r3]
	ldr	r2, [r3, #0]
	mov	r3, #-1073741824
	str	r2, [r3, #16]
	bl	lcd_incolor(PLT)
	ldr	r3, .L8+8
	ldr	r3, [r4, r3]
	ldr	r0, [r3, #0]
@ 291 "C:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 swi 6
@ 0 "" 2
	ldmfd	sp!, {r4, pc}
.L9:
	.align	2
.L8:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC3+8)
	.word	bfb(GOT)
	.word	framebuffer(GOT)
	.size	lcd_end, .-lcd_end
	.align	2
	.global	lcd_cmd
	.type	lcd_cmd, %function
lcd_cmd:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L22
	sub	r2, r0, #32
	and	r1, r2, #255
	cmp	r1, #31
.LPIC4:
	add	r3, pc, r3
	bls	.L21
	sub	r2, r0, #128
	and	r1, r2, #255
	cmp	r1, #63
	bhi	.L13
	ldr	r1, .L22+4
	ldr	r1, [r3, r1]
	str	r2, [r1, #0]
.L14:
	cmp	r0, #7
	addls	pc, pc, r0, asl #2
	b	.L10
.L20:
	b	.L15
	b	.L16
	b	.L17
	b	.L18
	b	.L19
	b	.L19
	b	.L19
	b	.L19
.L13:
	cmp	r0, #191
	bls	.L14
	sub	r0, r0, #192
	ldr	r2, .L22+8
	mov	r1, r0, asr #2
	rsb	r1, r1, #255
	ldr	r2, [r3, r2]
	mov	r0, r0, lsr #4
	and	r3, r1, #255
	and	r0, r0, #31
	mov	r3, r3, lsr #3
	mov	ip, r3, asl #10
	mov	r1, r0, asl #10
	ldr	r2, [r2, #0]
	orr	ip, ip, r3, asl #5
	orr	r1, r1, r0, asl #5
	orr	r3, ip, r3
	orr	r0, r1, r0
	orr	r3, r3, r0, asl #16
	str	r3, [r2, #0]
	bx	lr
.L19:
	ldr	r2, .L22+12
	ldr	r3, [r3, r2]
	strb	r0, [r3, #0]
.L10:
	bx	lr
.L21:
	ldr	r1, .L22+16
	ldr	r3, [r3, r1]
	str	r2, [r3, #0]
	bx	lr
.L18:
	ldr	r2, .L22+20
	ldr	r3, [r3, r2]
	mov	r2, #1
	strb	r2, [r3, #0]
	bx	lr
.L17:
	ldr	r2, .L22+20
	ldr	r3, [r3, r2]
	mov	r2, #0
	strb	r2, [r3, #0]
	bx	lr
.L16:
	ldr	r2, .L22+24
	ldr	r3, [r3, r2]
	mov	r2, #8
	str	r2, [r3, #0]
	bx	lr
.L15:
	ldr	r2, .L22+24
	ldr	r3, [r3, r2]
	mov	r2, #6
	str	r2, [r3, #0]
	bx	lr
.L23:
	.align	2
.L22:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC4+8)
	.word	cur_row(GOT)
	.word	palette(GOT)
	.word	auto_mode(GOT)
	.word	cur_col(GOT)
	.word	enabled(GOT)
	.word	n_bits(GOT)
	.size	lcd_cmd, .-lcd_cmd
	.align	2
	.global	lcd_cmd_read
	.type	lcd_cmd_read, %function
lcd_cmd_read:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L29
	ldr	r2, .L29+4
.LPIC5:
	add	r3, pc, r3
	ldr	r1, .L29+8
	ldr	r2, [r3, r2]
	ldr	r0, .L29+12
	ldr	r1, [r3, r1]
	ldrb	r2, [r2, #0]	@ zero_extendqisi2
	ldr	r3, [r3, r0]
	ldrb	r0, [r1, #0]	@ zero_extendqisi2
	ldr	r3, [r3, #0]
	cmp	r2, #7
	cmpne	r2, #5
	sub	r2, r2, #6
	movne	r1, #0
	moveq	r1, #1
	cmp	r2, #1
	movls	r2, #2
	movhi	r2, #0
	orr	r0, r1, r0, asl #5
	cmp	r3, #8
	orr	r0, r0, r2
	moveq	r3, #64
	movne	r3, #0
	and	r0, r0, #255
	orr	r0, r0, r3
	bx	lr
.L30:
	.align	2
.L29:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC5+8)
	.word	auto_mode(GOT)
	.word	enabled(GOT)
	.word	n_bits(GOT)
	.size	lcd_cmd_read, .-lcd_cmd_read
	.align	2
	.global	lcd_data
	.type	lcd_data, %function
lcd_data:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	ldr	r7, .L49
	ldr	r3, .L49+4
.LPIC6:
	add	r7, pc, r7
	ldr	r8, [r7, r3]
	ldr	r3, .L49+8
	ldr	r2, .L49+12
	ldr	sl, [r7, r3]
	ldr	r3, [r8, #0]
	ldr	r2, [r7, r2]
	ldr	r6, [sl, #0]
	sub	sp, sp, #12
	cmp	r3, #0
	str	r2, [sp, #4]
	ldr	fp, [r2, #0]
	mul	r6, r3, r6
	movle	r2, r3
	ble	.L32
	and	fp, fp, #63
	and	r9, r0, #255
	add	fp, fp, fp, asl #1
	add	r5, r6, r6, asl #1
	mov	r2, r3
	mov	r4, #0
	b	.L34
.L33:
	add	r4, r4, #1
	cmp	r4, r2
	add	r5, r5, #3
	mov	r3, r2
	bge	.L32
.L34:
	add	r1, r4, r6
	cmp	r1, #95
	bgt	.L33
	ldr	r2, .L49+16
	sub	r3, r3, #1
	ldr	r1, [r7, r2]
	rsb	r2, r4, r3
	ldr	r3, [r1, #0]
	mov	r1, #1
	ands	r2, r9, r1, asl r2
	add	r3, r3, #7680
	moveq	r2, #0
	movne	r2, #1
	mov	r0, r5
	add	r3, r3, #16
	mov	r1, fp
	bl	_n_set_84_pixel(PLT)
	ldr	r2, [r8, #0]
	add	r4, r4, #1
	cmp	r4, r2
	add	r5, r5, #3
	mov	r3, r2
	blt	.L34
.L32:
	ldr	r3, .L49+20
	ldr	r3, [r7, r3]
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	sub	r3, r3, #4
	cmp	r3, #3
	addls	pc, pc, r3, asl #2
	b	.L31
.L40:
	b	.L36
	b	.L37
	b	.L38
	b	.L39
.L39:
	ldr	r3, [sl, #0]
	cmp	r2, #8
	add	r3, r3, #1
	movne	r2, #20
	moveq	r2, #15
	cmp	r2, r3
	str	r3, [sl, #0]
	beq	.L48
	cmp	r3, #32
	bne	.L31
.L48:
	mov	r3, #0
	str	r3, [sl, #0]
.L31:
	add	sp, sp, #12
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
.L38:
	ldr	r3, [sl, #0]
	sub	r3, r3, #1
	cmn	r3, #1
	str	r3, [sl, #0]
	bne	.L31
	cmp	r2, #8
	movne	r2, #19
	moveq	r2, #14
	str	r2, [sl, #0]
	b	.L31
.L37:
	ldr	r1, [sp, #4]
	ldr	r3, [r1, #0]
	add	r3, r3, #1
	str	r3, [r1, #0]
	b	.L31
.L36:
	ldr	r2, [sp, #4]
	ldr	r3, [r2, #0]
	sub	r3, r3, #1
	str	r3, [r2, #0]
	b	.L31
.L50:
	.align	2
.L49:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC6+8)
	.word	n_bits(GOT)
	.word	cur_col(GOT)
	.word	cur_row(GOT)
	.word	framebuffer(GOT)
	.word	auto_mode(GOT)
	.size	lcd_data, .-lcd_data
	.align	2
	.global	lcd_data_read
	.type	lcd_data_read, %function
lcd_data_read:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L52
	ldr	r2, .L52+4
.LPIC7:
	add	r3, pc, r3
	ldr	ip, [r3, r2]
	ldr	r2, .L52+8
	ldr	r0, [r3, r2]
	ldr	r2, .L52+12
	ldr	r0, [r0, #0]
	ldr	r1, [r3, r2]
	ldr	r2, .L52+16
	ldr	r1, [r1, #0]
	ldr	r2, [r3, r2]
	ldr	r3, [ip, #0]
	ldr	r2, [r2, #0]
	mla	r3, r0, r3, r1
	add	r2, r2, r2, asl #2
	add	r3, r3, r2, asl #6
	ldr	r2, .L52+20
	ldrb	r1, [r3, r2]	@ zero_extendqisi2
	add	r2, r2, #1
	ldrb	r0, [r3, r2]	@ zero_extendqisi2
	add	r2, r2, #1
	orr	r1, r1, r0, asl #1
	ldr	r0, .L52+24
	ldrb	r2, [r3, r2]	@ zero_extendqisi2
	ldrb	r0, [r3, r0]	@ zero_extendqisi2
	and	r1, r1, #255
	orr	r1, r1, r2, asl #2
	ldr	r2, .L52+28
	and	r1, r1, #255
	orr	r1, r1, r0, asl #3
	ldr	r0, .L52+32
	ldrb	r2, [r3, r2]	@ zero_extendqisi2
	ldrb	r0, [r3, r0]	@ zero_extendqisi2
	and	r1, r1, #255
	orr	r1, r1, r2, asl #4
	and	r1, r1, #255
	ldr	r2, .L52+36
	orr	r1, r1, r0, asl #5
	ldr	r0, .L52+40
	ldrb	r2, [r3, r2]	@ zero_extendqisi2
	ldrb	r0, [r3, r0]	@ zero_extendqisi2
	and	r3, r1, #255
	orr	r3, r3, r2, asl #6
	orr	r0, r3, r0, asl #7
	and	r0, r0, #255
	bx	lr
.L53:
	.align	2
.L52:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC7+8)
	.word	cur_col(GOT)
	.word	n_bits(GOT)
	.word	framebuffer(GOT)
	.word	cur_row(GOT)
	.word	7696
	.word	7699
	.word	7700
	.word	7701
	.word	7702
	.word	7703
	.size	lcd_data_read, .-lcd_data_read
	.global	px_offsets
	.global	palette
	.comm	bfb,4,4
	.comm	framebuffer,4,4
	.global	enabled
	.global	auto_mode
	.global	n_bits
	.global	row_offset
	.global	cur_col
	.global	cur_row
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
.LC0:
	.word	-1073741796
	.word	-1073741800
	.data
	.align	2
	.type	px_offsets, %object
	.size	px_offsets, 18
px_offsets:
	.short	0
	.short	1
	.short	2
	.short	320
	.short	321
	.short	322
	.short	640
	.short	641
	.short	642
	.space	2
	.type	palette, %object
	.size	palette, 4
palette:
	.word	-1073741312
	.type	enabled, %object
	.size	enabled, 1
enabled:
	.byte	1
	.type	auto_mode, %object
	.size	auto_mode, 1
auto_mode:
	.byte	5
	.space	2
	.type	n_bits, %object
	.size	n_bits, 4
n_bits:
	.word	8
	.bss
	.align	2
	.type	row_offset, %object
	.size	row_offset, 4
row_offset:
	.space	4
	.type	cur_col, %object
	.size	cur_col, 4
cur_col:
	.space	4
	.type	cur_row, %object
	.size	cur_row, 4
cur_row:
	.space	4
	.ident	"GCC: (GNU) 4.6.2"
