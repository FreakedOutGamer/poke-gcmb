@ Thin Thumb veneers for the GBA BIOS software interrupts these images use.

	.syntax unified
	.section .text, "ax"
	.thumb
	.align 1, 0

	.thumb_func
CpuSet:
	swi 0xB
	bx lr

	.thumb_func
Div:
	swi 0x6
	bx lr

	.thumb_func
Mod:
	swi 0x6
	adds r0, r1, #0
	bx lr
	.align 2, 0

	.thumb_func
HuffUnComp:
	swi 0x13
	bx lr

	.thumb_func
LZ77UnCompVram:
	swi 0x12
	bx lr

	.thumb_func
LZ77UnCompWram:
	swi 0x11
	bx lr

	.thumb_func
RLUnCompVram:
	swi 0x15
	bx lr

	.thumb_func
RegisterRamReset:
	swi 0x1
	bx lr

	.thumb_func
SoftReset:
	ldr r3, =0x04000208
	movs r2, #0
	strb r2, [r3]
	ldr r1, =0x03007F00 @ user stack
	mov sp, r1
	swi 0x1
	swi 0
	.align 2, 0
	.pool

	.thumb_func
VBlankIntrWait:
	movs r2, #0
	swi 0x5
	bx lr
	.align 2, 0

	.thumb_func
SoftResetRom:
	ldr r3, =0x04000208
	movs r2, #0
	strb r2, [r3]
	ldr r3, =0x03007FFA
	movs r2, #1
	strb r2, [r3]
	subs r3, #0xFA
	mov sp, r3
	movs r2, #1
	bics r0, r2
	swi 0x1
	swi 0
	.align 2, 0
	.pool
