	.include "asm/macros/function.inc"

	.section .text.libagbsyscall, "ax"
	.syntax unified

	thumb_func_start CpuSet
CpuSet:
	svc 0xB
	bx lr
	thumb_func_end CpuSet

	thumb_func_start HuffUnComp
HuffUnComp:
	svc 0x13
	bx lr
	thumb_func_end HuffUnComp

	thumb_func_start LZ77UnCompWram
LZ77UnCompWram:
	svc 0x11
	bx lr
	thumb_func_end LZ77UnCompWram

	thumb_func_start RegisterRamReset
RegisterRamReset:
	svc 0x1
	bx lr
	thumb_func_end RegisterRamReset

	thumb_func_start SoftReset
SoftReset:
	ldr r3, =0x04000208 @ REG_IME
	movs r2, #0
	strb r2, [r3]
	ldr r1, =0x03007F00 @ the user stack's top
	mov sp, r1
	svc 0x1 @ RegisterRamReset
	svc 0x0 @ SoftReset
	.pool
	thumb_func_end SoftReset

	thumb_func_start VBlankIntrWait
VBlankIntrWait:
	movs r2, #0
	svc 0x5
	bx lr
	thumb_func_end VBlankIntrWait

@ SoftReset that restarts from the cartridge (0x08000000): it clears the
@ BIOS's return-address flag at 0x03007FFA first.
	thumb_func_start SoftResetRom
SoftResetRom:
	ldr r3, =0x04000208 @ REG_IME
	movs r2, #0
	strb r2, [r3]
	ldr r3, =0x03007FFA
	movs r2, #0
	strb r2, [r3]
	subs r3, #0xFA
	mov sp, r3
	svc 0x1 @ RegisterRamReset
	svc 0x0 @ SoftReset
	.pool
	thumb_func_end SoftResetRom
