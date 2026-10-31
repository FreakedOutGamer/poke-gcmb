@ Thin Thumb veneers for the GBA BIOS software interrupts these images use.

	.syntax unified
	.section .text, "ax"
	.thumb
	.align 1, 0

	.global CpuSet
	.thumb_func
CpuSet:
	swi 0xB
	bx lr

	.global Div
	.thumb_func
Div:
	swi 0x6
	bx lr

	.global Mod
	.thumb_func
Mod:
	swi 0x6
	adds r0, r1, #0
	bx lr
	.align 2, 0

	.global HuffUnComp
	.thumb_func
HuffUnComp:
	swi 0x13
	bx lr

	.global LZ77UnCompVram
	.thumb_func
LZ77UnCompVram:
	swi 0x12
	bx lr

	.global RLUnCompVram
	.thumb_func
RLUnCompVram:
	swi 0x15
	bx lr

	.global RegisterRamReset
	.thumb_func
RegisterRamReset:
	swi 0x1
	bx lr

	.global SoftReset
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

	.global VBlankIntrWait
	.thumb_func
VBlankIntrWait:
	movs r2, #0
	swi 0x5
	bx lr
	.align 2, 0
