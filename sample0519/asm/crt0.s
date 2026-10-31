@ The payload's start: its own AGB header (the loader jumps to its first
@ word), then the SDK's crt0: the entry point, and the interrupt dispatcher
@ Init installs. This dispatcher nests: it re-enables interrupts, keeps only
@ the gamepak, serial, timer 3 and H-blank interrupts on while a handler
@ runs, and runs the handler in system mode.

	.text
	.syntax unified
	.arm
	.align 2, 0

	.global Start
Start:
	b Init

	.global RomHeaderNintendoLogo
RomHeaderNintendoLogo:
	.incbin "../common/nintendo_logo.bin"

RomHeaderGameTitle:
	.space 12
RomHeaderGameCode:
	.space 4
RomHeaderMakerCode:
	.ascii "01"
RomHeaderMagic:
	.byte 0x96
RomHeaderMainUnitCode:
	.byte 0
RomHeaderDeviceType:
	.byte 0
RomHeaderReserved1:
	.space 7
RomHeaderSoftwareVersion:
	.byte 0
RomHeaderChecksum:
	.byte 0xF0
RomHeaderReserved2:
	.space 2

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

sp_sys:      .4byte 0x03007F00
sp_irq:      .4byte 0x03007FA0
intr_vector: .4byte 0x03007FFC
agb_main:    .4byte AgbMain + 1

	.global IntrMain
IntrMain:
	ldr r3, reg_ie
	ldr r2, [r3] @ REG_IE | REG_IF << 16
	ldrh r1, [r3, #8] @ REG_IME
	mrs r0, spsr
	push {r0, r1, r2, r3, lr}
	mov r0, #1
	strh r0, [r3, #8] @ re-enable interrupts, so a higher one can nest
	and r1, r2, r2, lsr #16 @ interrupts both enabled and requested
	ldr ip, intr_table
	ands r0, r1, #0xC0 @ INTR_FLAG_TIMER3 | INTR_FLAG_SERIAL
	bne .Ldispatch
	add ip, ip, #4
	ands r0, r1, #0x2 @ INTR_FLAG_HBLANK
	bne .Ldispatch
	add ip, ip, #4
	ands r0, r1, #0x4 @ INTR_FLAG_VCOUNT
	bne .Ldispatch
	add ip, ip, #4
	ands r0, r1, #0x1 @ INTR_FLAG_VBLANK
	bne .Ldispatch
	add ip, ip, #4
	ands r0, r1, #0x100 @ INTR_FLAG_DMA0
	bne .Ldispatch
	add ip, ip, #4
	ands r0, r1, #0x20 @ INTR_FLAG_TIMER2
	bne .Ldispatch
	add ip, ip, #4
	ands r0, r1, #0x400 @ INTR_FLAG_DMA2
	bne .Ldispatch
	add ip, ip, #4
	ands r0, r1, #0x800 @ INTR_FLAG_DMA3
	bne .Ldispatch
	add ip, ip, #4
	ands r0, r1, #0x1000 @ INTR_FLAG_KEYPAD
	bne .Ldispatch
	add ip, ip, #4
	ands r0, r1, #0x2000 @ INTR_FLAG_GAMEPAK
	strbne r0, [r3, #-0x17C] @ REG_SOUNDCNT_X: sound off
.Lgamepak_removed:
	bne .Lgamepak_removed @ the cartridge was pulled: hang
.Ldispatch:
	strh r0, [r3, #2] @ acknowledge in REG_IF
	ldr r1, nest_mask
	bic r2, r2, r0
	and r1, r1, r2
	strh r1, [r3] @ REG_IE while the handler runs
	mrs r3, cpsr
	bic r3, r3, #0xDF
	orr r3, r3, #0x1F @ PSR_SYS_MODE
	msr cpsr_fc, r3
	ldr r0, [ip]
	stmfd sp!, {lr}
	add lr, pc, #0
	bx r0
	ldmfd sp!, {lr}
	mrs r3, cpsr
	bic r3, r3, #0xDF
	orr r3, r3, #0x92 @ PSR_IRQ_MODE, IRQs disabled
	msr cpsr_fc, r3
	pop {r0, r1, r2, r3, lr}
	strh r2, [r3] @ restore REG_IE
	strh r1, [r3, #8] @ and REG_IME
	msr spsr_fc, r0
	bx lr

reg_ie:     .4byte 0x04000200 @ REG_IE
intr_table: .4byte gIntrTable
nest_mask:  .4byte 0x20C2 @ GAMEPAK, SERIAL, TIMER3 and HBLANK
