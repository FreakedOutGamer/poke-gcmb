	.include "asm/macros/function.inc"

	.section .text.libagbsyscall, "ax"
	.syntax unified

	thumb_func_start CpuSet
CpuSet:
	svc 0xB
	bx lr
	thumb_func_end CpuSet

	thumb_func_start Diff16bitUnFilter
Diff16bitUnFilter:
	svc 0x18
	bx lr
	thumb_func_end Diff16bitUnFilter

	thumb_func_start Diff8bitUnFilterVram
Diff8bitUnFilterVram:
	svc 0x17
	bx lr
	thumb_func_end Diff8bitUnFilterVram

	thumb_func_start Diff8bitUnFilterWram
Diff8bitUnFilterWram:
	svc 0x16
	bx lr
	thumb_func_end Diff8bitUnFilterWram

	thumb_func_start HuffUnComp
HuffUnComp:
	svc 0x13
	bx lr
	thumb_func_end HuffUnComp

	thumb_func_start LZ77UnCompVram
LZ77UnCompVram:
	svc 0x12
	bx lr
	thumb_func_end LZ77UnCompVram

	thumb_func_start LZ77UnCompWram
LZ77UnCompWram:
	svc 0x11
	bx lr
	thumb_func_end LZ77UnCompWram

	thumb_func_start RLUnCompVram
RLUnCompVram:
	svc 0x15
	bx lr
	thumb_func_end RLUnCompVram

	thumb_func_start RLUnCompWram
RLUnCompWram:
	svc 0x14
	bx lr
	thumb_func_end RLUnCompWram

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
