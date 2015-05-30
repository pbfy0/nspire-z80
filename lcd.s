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
	
_n_set_84_pixel:
	push {r4-r8}
	mov r4, #480
	mla r3, r1, r4, r3
	mov r4, #3
	mul r4, r0, r4
	add r3, r3, r4, asr #1
	mov r4, r3
	mov r5, #15
	mov r6, r2
	lsl r6, #4
	orr r6, r2
	tst r0, #1
	addne r3, #1
	addeq r4, #1
	lsleq r5, #4
	lsleq r2, #4
	mov r7, #3
loop_begin:
	strb r6, [r3]
	ldrb r8, [r4]
	bic r8, r5
	orr r8, r2
	strb r8, [r4]
	add r3, r3, #160
	add r4, r4, #160
	subs r7, r7, #1
	bne loop_begin
	pop {r4-r8}
	
	.text
	.align	2
	.global	lcd_init
	.type	lcd_init, %function
lcd_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L5
	ldr	r2, .L5+4
.LPIC0:
	add	r3, pc, r3
	stmfd	sp!, {r4, r5, r6, lr}
	ldr	r5, [r3, r2]
	mov	r3, #-1073741824
	ldr	r3, [r3, #16]
	str	r3, [r5, #0]
	bl	lcd_ingray(PLT)
	ldr	r0, [r5, #0]
	mov	r1, #170
	mov	r2, #38400
@ 294 "c:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 swi 7
@ 0 "" 2
	ldr	r4, .L5+8
	ldr	r6, .L5+12
.L2:
	ldr	r0, [r5, #0]
	mov	r1, #255
	add	r0, r0, r4, asr #1
	mov	r2, #144
@ 294 "c:/Users/Paul/ndless-sdk-v3.6-r53.464a532/ndless/bin/../include/os.h" 1
	 swi 7
@ 0 "" 2
	add	r4, r4, #320
	cmp	r4, r6
	bne	.L2
	ldmfd	sp!, {r4, r5, r6, pc}
.L6:
	.align	2
.L5:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC0+8)
	.word	framebuffer(GOT)
	.word	7696
	.word	69136
	.size	lcd_init, .-lcd_init
	.align	2
	.global	lcd_cmd
	.type	lcd_cmd, %function
lcd_cmd:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L20
	sub	r2, r0, #32
	and	r1, r2, #255
	cmp	r1, #31
.LPIC1:
	add	r3, pc, r3
	bls	.L19
	sub	r2, r0, #128
	and	r1, r2, #255
	cmp	r1, #63
	bhi	.L10
	ldr	r1, .L20+4
	ldr	r1, [r3, r1]
	str	r2, [r1, #0]
.L11:
	cmp	r0, #7
	addls	pc, pc, r0, asl #2
	b	.L7
.L16:
	b	.L12
	b	.L12
	b	.L13
	b	.L14
	b	.L15
	b	.L15
	b	.L15
	b	.L15
.L10:
	cmp	r0, #191
	bxhi	lr
	b	.L11
.L15:
	ldr	r2, .L20+8
	ldr	r3, [r3, r2]
	strb	r0, [r3, #0]
.L7:
	bx	lr
.L19:
	ldr	r1, .L20+12
	ldr	r3, [r3, r1]
	str	r2, [r3, #0]
	bx	lr
.L14:
	ldr	r2, .L20+16
	ldr	r3, [r3, r2]
	mov	r2, #1
	strb	r2, [r3, #0]
	bx	lr
.L13:
	ldr	r2, .L20+16
	ldr	r3, [r3, r2]
	mov	r2, #0
	strb	r2, [r3, #0]
	bx	lr
.L12:
	ldr	r2, .L20+20
	cmp	r0, #0
	ldr	r3, [r3, r2]
	moveq	r0, #6
	movne	r0, #8
	str	r0, [r3, #0]
	bx	lr
.L21:
	.align	2
.L20:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC1+8)
	.word	cur_row(GOT)
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
	ldr	r3, .L27
	ldr	r2, .L27+4
.LPIC2:
	add	r3, pc, r3
	ldr	r1, .L27+8
	ldr	r2, [r3, r2]
	ldr	r0, .L27+12
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
.L28:
	.align	2
.L27:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC2+8)
	.word	auto_mode(GOT)
	.word	enabled(GOT)
	.word	n_bits(GOT)
	.size	lcd_cmd_read, .-lcd_cmd_read
	.align	2
	.global	lcd_data
	.type	lcd_data, %function
lcd_data:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	ldr	r9, .L49
	ldr	r3, .L49+4
.LPIC3:
	add	r9, pc, r9
	ldr	r3, [r9, r3]
	sub	sp, sp, #20
	str	r3, [sp, #0]
	ldr	r3, .L49+8
	ldr	r1, [sp, #0]
	ldr	r3, [r9, r3]
	ldr	r7, [r1, #0]
	str	r3, [sp, #8]
	ldr	r5, [r3, #0]
	ldr	r3, .L49+12
	cmp	r7, #0
	ldr	r3, [r9, r3]
	mul	r5, r7, r5
	str	r3, [sp, #12]
	ldr	fp, [r3, #0]
	movle	r3, r7
	ble	.L30
	ldr	r2, .L49+16
	and	fp, fp, #63
	rsb	r4, fp, fp, asl #4
	and	ip, r0, #255
	ldr	r8, [r9, r2]
	add	r4, r5, r4, asl #3
	str	r9, [sp, #4]
	mov	r3, r7
	mov	r6, #0
	mov	sl, #1
	mov	r9, ip
.L35:
	sub	r7, r7, #1
	rsb	r7, r6, r7
	cmp	r5, #95
	mov	r0, r5
	mov	r1, fp
	and	r7, r9, sl, asl r7
	bgt	.L31
	ldr	r2, [sp, #4]
	ldr	r3, .L49+20
	cmp	r7, #0
	ldr	r3, [r2, r3]
	moveq	r2, #15
	ldr	r3, [r3, #0]
	movne	r2, #0
	add	r3, r3, #3840
	add	r3, r3, #8
	bl	_n_set_84_pixel(PLT)
	ldr	r1, [sp, #0]
	ldr	r3, [r1, #0]
.L31:
	cmp	r7, #0
	and	r1, r4, #7
	mov	r2, r1
	ldrneb	r2, [r8, r4, asr #3]	@ zero_extendqisi2
	ldreqb	r1, [r8, r4, asr #3]	@ zero_extendqisi2
	orrne	r1, r2, sl, asl r1
	biceq	r2, r1, r2
	add	r6, r6, #1
	strneb	r1, [r8, r4, asr #3]
	streqb	r2, [r8, r4, asr #3]
	cmp	r6, r3
	add	r5, r5, #1
	mov	r7, r3
	add	r4, r4, #1
	blt	.L35
	ldr	r9, [sp, #4]
.L30:
	ldr	r2, .L49+24
	ldr	r2, [r9, r2]
	ldrb	r2, [r2, #0]	@ zero_extendqisi2
	sub	r2, r2, #4
	cmp	r2, #3
	addls	pc, pc, r2, asl #2
	b	.L29
.L41:
	b	.L37
	b	.L38
	b	.L39
	b	.L40
.L40:
	ldr	r1, [sp, #8]
	cmp	r3, #8
	ldr	r2, [r1, #0]
	movne	r3, #20
	moveq	r3, #15
	add	r2, r2, #1
	cmp	r3, r2
	moveq	r3, #0
	str	r2, [r1, #0]
	streq	r3, [r1, #0]
.L29:
	add	sp, sp, #20
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
.L39:
	ldr	r1, [sp, #8]
	ldr	r2, [r1, #0]
	sub	r2, r2, #1
	cmn	r2, #1
	str	r2, [r1, #0]
	bne	.L29
	cmp	r3, #8
	movne	r3, #19
	moveq	r3, #14
	str	r3, [r1, #0]
	b	.L29
.L38:
	ldr	r1, [sp, #12]
	ldr	r3, [r1, #0]
	add	r3, r3, #1
	str	r3, [r1, #0]
	b	.L29
.L37:
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #0]
	sub	r3, r3, #1
	str	r3, [r2, #0]
	b	.L29
.L50:
	.align	2
.L49:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC3+8)
	.word	n_bits(GOT)
	.word	cur_col(GOT)
	.word	cur_row(GOT)
	.word	video_mem(GOT)
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
.LPIC4:
	add	r3, pc, r3
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
	ldr	r1, [r3, r2]
	ldr	r2, .L52+8
	ldr	r1, [r1, #0]
	ldr	r0, [r3, r2]
	ldr	r2, .L52+12
	ldr	r0, [r0, #0]
	ldr	r2, [r3, r2]
	mul	r1, r0, r1
	ldr	r2, [r2, #0]
	ldr	r0, .L52+16
	rsb	r2, r2, r2, asl #4
	ldr	r3, [r3, r0]
	mov	r2, r2, asl #3
	add	r8, r1, #1
	add	sl, r1, r2
	add	r8, r8, r2
	add	r0, r1, #2
	ldrb	r9, [r3, sl, asr #3]	@ zero_extendqisi2
	ldrb	r7, [r3, r8, asr #3]	@ zero_extendqisi2
	add	r0, r0, r2
	add	r5, r1, #3
	ldrb	r6, [r3, r0, asr #3]	@ zero_extendqisi2
	add	r5, r5, r2
	add	r4, r1, #4
	ldrb	fp, [r3, r5, asr #3]	@ zero_extendqisi2
	add	r4, r4, r2
	add	ip, r1, #5
	and	sl, sl, #7
	and	r8, r8, #7
	add	ip, ip, r2
	mov	sl, r9, asr sl
	mov	r8, r7, asr r8
	and	r9, r0, #7
	ldrb	r7, [r3, r4, asr #3]	@ zero_extendqisi2
	add	r0, r1, #6
	add	r0, r0, r2
	mov	r9, r6, asr r9
	and	r5, r5, #7
	ldrb	r6, [r3, ip, asr #3]	@ zero_extendqisi2
	add	r1, r1, #7
	add	r1, r1, r2
	and	r8, r8, #1
	mov	r5, fp, asr r5
	and	sl, sl, #1
	ldrb	fp, [r3, r0, asr #3]	@ zero_extendqisi2
	and	r4, r4, #7
	ldrb	r2, [r3, r1, asr #3]	@ zero_extendqisi2
	orr	sl, sl, r8, asl #1
	and	r9, r9, #1
	mov	r4, r7, asr r4
	and	ip, ip, #7
	orr	sl, sl, r9, asl #2
	mov	r6, r6, asr ip
	and	r3, r0, #7
	and	r5, r5, #1
	orr	r5, sl, r5, asl #3
	and	r4, r4, #1
	mov	r3, fp, asr r3
	and	r1, r1, #7
	orr	r7, r5, r4, asl #4
	mov	r0, r2, asr r1
	and	r6, r6, #1
	orr	r6, r7, r6, asl #5
	and	r3, r3, #1
	orr	r3, r6, r3, asl #6
	and	r0, r0, #1
	orr	r0, r3, r0, asl #7
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
	bx	lr
.L53:
	.align	2
.L52:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC4+8)
	.word	cur_col(GOT)
	.word	n_bits(GOT)
	.word	cur_row(GOT)
	.word	video_mem(GOT)
	.size	lcd_data_read, .-lcd_data_read
	.comm	framebuffer,4,4
	.comm	video_mem,960,4
	.global	enabled
	.global	auto_mode
	.global	n_bits
	.global	row_offset
	.global	cur_col
	.global	cur_row
	.data
	.align	2
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
