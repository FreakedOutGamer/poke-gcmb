@ The SDK's crt0.s: the payload's entry point, and the ARM interrupt
@ dispatcher AgbMain copies to IWRAM (IntrMain_Buffer). This is the older
@ SDK dispatcher: it runs one handler per interrupt in IRQ mode, with no
@ nesting. It is the same in both revisions.

	.text
	.syntax unified
	.arm
	.align 2, 0

	.global Init
Init:
	mov r0, #0x12 @ PSR_IRQ_MODE
	msr cpsr_fc, r0
	ldr sp, sp_irq
	mov r0, #0x1F @ PSR_SYS_MODE
	msr cpsr_fc, r0
	ldr sp, sp_sys
	ldr r1, intr_vector
	adr r0, IntrMain
	str r0, [r1]
	ldr r1, agb_main
	mov lr, pc
	bx r1
	b Init

sp_sys: .4byte 0x03007F00
sp_irq: .4byte 0x03007FA0

	.global IntrMain
IntrMain:
	mov ip, #0x4000000 @ REG_BASE
	add r3, ip, #0x200 @ REG_IE
	ldr r2, [r3] @ REG_IE | REG_IF << 16
	and r1, r2, r2, lsr #16 @ interrupts both enabled and requested
	mov r2, #0 @ offset into gIntrTable
	ands r0, r1, #0x2000 @ INTR_FLAG_GAMEPAK
	strbne r0, [r3, #-0x17C] @ REG_SOUNDCNT_X: sound off
.Lgamepak_removed:
	bne .Lgamepak_removed @ the cartridge was pulled: hang
	ands r0, r1, #0xC0 @ INTR_FLAG_TIMER3 | INTR_FLAG_SERIAL
	bne .Ldispatch
	add r2, r2, #4
	ands r0, r1, #0x1 @ INTR_FLAG_VBLANK
	strhne r0, [ip, #-8] @ INTR_CHECK, for VBlankIntrWait
	bne .Ldispatch
	add r2, r2, #4
	ands r0, r1, #0x2 @ INTR_FLAG_HBLANK
	bne .Ldispatch
	add r2, r2, #4
	ands r0, r1, #0x4 @ INTR_FLAG_VCOUNT
	bne .Ldispatch
	add r2, r2, #4
	ands r0, r1, #0x100 @ INTR_FLAG_DMA0
	bne .Ldispatch
	add r2, r2, #4
	ands r0, r1, #0x200 @ INTR_FLAG_DMA1
	bne .Ldispatch
	add r2, r2, #4
	ands r0, r1, #0x400 @ INTR_FLAG_DMA2
	bne .Ldispatch
	add r2, r2, #4
	ands r0, r1, #0x800 @ INTR_FLAG_DMA3
	bne .Ldispatch
	add r2, r2, #4
	ands r0, r1, #0x1000 @ INTR_FLAG_KEYPAD
	bne .Ldispatch
	add r2, r2, #4
	ands r0, r1, #0x20 @ INTR_FLAG_TIMER2
.Ldispatch:
	strh r0, [r3, #2] @ acknowledge in REG_IF
	ldr r1, intr_table
	add r1, r1, r2
	ldr r0, [r1]
	bx r0

intr_vector: .4byte 0x03007FFC
agb_main:    .4byte AgbMain + 1
intr_table:  .4byte gIntrTable
