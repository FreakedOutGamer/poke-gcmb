	.include "asm/macros/function.inc"
	.syntax unified

@ Nintendo MultiSio library (MultiSio010918): ARM routines.
@ MultiSioInit copies these into IWRAM. Ported from the Aura Mew
@ distribution decomp, which uses the same library version.

	.text

	arm_func_start MultiSioRecvBufChange
MultiSioRecvBufChange:
	push {r4, r5, r6, r7, r8, sb, sl, fp}
	mov ip, #0x4000000
	ldr fp, .Lmsio_0210 @ =gMultiSioArea
	add sl, fp, #0x40
	mov sb, #1
	mov r8, #0
	strb r8, [ip, #0x208]
	ldm sl, {r0, r1, r2, r3, r4, r5, r6, r7}
	stm sl!, {r4, r5, r6, r7}
	stm sl!, {r0, r1, r2, r3}
	ldr r0, [fp, #4]
	str r8, [fp, #4]
	strb sb, [ip, #0x208]
	pop {r4, r5, r6, r7, r8, sb, sl, fp}
	bx lr
	.align 2, 0
.Lmsio_0210: .4byte gMultiSioArea

	arm_func_start MultiSioIntr
MultiSioIntr:
	mov ip, #0x4000000
	add ip, ip, #0x120
	ldm ip, {r0, r1}
	push {r7, r8, sb, sl, fp}
	ldr fp, .Lmsio_032C @ =gMultiSioArea
	mov sb, #0xfe
	add sb, sb, #0xfe00
	ldrh r3, [ip, #8]
	and r3, r3, #0x40
	strb r3, [fp, #9]
	ldr sl, [fp, #0x14]
	adds r3, sl, #1
	blt .Lmsio_0270
	bne .Lmsio_0264
	strh sb, [ip, #0xa]
	add r8, fp, #0x28
	ldm r8, {r2, r3}
	mov r7, r2
	stm r8, {r3, r7}
	b .Lmsio_0270
.Lmsio_0264:
	ldr r3, [fp, #0x2c]
	ldr r2, [r3, sl, lsl #1]
	strh r2, [ip, #0xa]
.Lmsio_0270:
	cmp sl, #0xb
	addlt sl, sl, #1
	strlt sl, [fp, #0x14]
	push {r0, r1, r5, r6}
	mov r6, #3
.Lmsio_0284:
	add r8, fp, #0x18
	add r8, r8, r6, lsl #2
	ldr sl, [r8]
	lsl r3, r6, #1
	ldrh r5, [sp, r3]
	cmp r5, sb
	bne .Lmsio_02B4
	cmp sl, #9
	ble .Lmsio_02B4
	mov r0, #1
	sub sl, r0, #2
	b .Lmsio_02E0
.Lmsio_02B4:
	ldr r0, [r8, #0x18]
	lsl r3, sl, #1
	strh r5, [r0, r3]
	cmp sl, #9
	bne .Lmsio_02E0
	ldr r1, [r8, #0x28]
	str r0, [r8, #0x28]
	str r1, [r8, #0x18]
	add r3, fp, #4
	mov r0, #1
	strb r0, [r3, r6]
.Lmsio_02E0:
	cmp sl, #0xb
	addlt sl, sl, #1
	str sl, [r8]
	subs r6, r6, #1
	bge .Lmsio_0284
	ldrb r0, [fp]
	cmp r0, #0
	beq .Lmsio_0320
	ldr r7, .Lmsio_0330 @ =0x0400010E
	mov r0, #0
	strh r0, [r7]
	ldrh r0, [ip, #8]
	orr r0, r0, #0x80
	strh r0, [ip, #8]
	mov r0, #0xc0
	strh r0, [r7]
.Lmsio_0320:
	add sp, sp, #8
	pop {r5, r6, r7, r8, sb, sl, fp}
	bx lr
	.align 2, 0
.Lmsio_032C: .4byte gMultiSioArea
.Lmsio_0330: .4byte 0x0400010E

