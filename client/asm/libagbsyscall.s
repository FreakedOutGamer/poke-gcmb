	.section .text.libagbsyscall, "ax"
	.syntax unified
	.thumb

	@ Each stub starts word-aligned, as with pret's thumb_func_start.

	.align 2, 0
	.global CpuSet
	.thumb_func
CpuSet:
	svc 0xB
	bx lr

	.if REVISION == 1
	.align 2, 0
	.global Div
	.thumb_func
Div:
	svc 0x6
	bx lr

	.align 2, 0
	.global Mod
	.thumb_func
Mod:
	svc 0x6
	adds r0, r1, #0
	bx lr
	.endif

	.align 2, 0
	.global RegisterRamReset
	.thumb_func
RegisterRamReset:
	svc 0x1
	bx lr

	.align 2, 0
	.global VBlankIntrWait
	.thumb_func
VBlankIntrWait:
	movs r2, #0
	svc 0x5
	bx lr

	.align 2, 0
