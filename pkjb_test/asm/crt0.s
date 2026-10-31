	.section .crt0, "ax"
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

sp_sys:      .4byte 0x03007E60
sp_irq:      .4byte 0x03007FA0
intr_vector: .4byte 0x03007FFC
agb_main:    .4byte AgbMain + 1

	.global IntrMain
IntrMain:
	mov r3, #0x04000000 @ REG_BASE
	add r3, r3, #0x200 @ REG_IE
	ldr r2, [r3] @ REG_IE | REG_IF << 16
	ldrh r1, [r3, #8] @ REG_IME
	mrs r0, spsr
	push {r0, r1, r2, r3, lr}
	mov r0, #1
	strh r0, [r3, #8] @ re-enable interrupts, so a higher one can nest
	and r1, r2, r2, lsr #16 @ interrupts both enabled and requested
	mov ip, #0 @ offset into gIntrTable

	@ pkjb_test checks the IRQ bits in numerical order. This differs from
	@ pokechan/colosseum-mb, which gives serial IRQ priority first.
	ands r0, r1, #0x01
	bne .Ldispatch_irq
	add ip, ip, #4
	ands r0, r1, #0x02
	bne .Ldispatch_irq
	add ip, ip, #4
	ands r0, r1, #0x04
	bne .Ldispatch_irq
	add ip, ip, #4
	ands r0, r1, #0x08
	bne .Ldispatch_irq
	add ip, ip, #4
	ands r0, r1, #0x10
	bne .Ldispatch_irq
	add ip, ip, #4
	ands r0, r1, #0x20
	bne .Ldispatch_irq
	add ip, ip, #4
	ands r0, r1, #0x40
	bne .Ldispatch_irq
	add ip, ip, #4
	ands r0, r1, #0x80
	bne .Ldispatch_irq
	add ip, ip, #4
	ands r0, r1, #0x100
	bne .Ldispatch_irq
	add ip, ip, #4
	ands r0, r1, #0x200
	bne .Ldispatch_irq
	add ip, ip, #4
	ands r0, r1, #0x400
	bne .Ldispatch_irq
	add ip, ip, #4
	ands r0, r1, #0x800
	bne .Ldispatch_irq
	add ip, ip, #4
	ands r0, r1, #0x1000
	bne .Ldispatch_irq
	add ip, ip, #4
	ands r0, r1, #0x2000
	strbne r0, [r3, #-0x17C] @ REG_SOUNDCNT_X: sound off
.Lgamepak_removed:
	bne .Lgamepak_removed @ the cartridge was pulled: hang

.Ldispatch_irq:
	strh r0, [r3, #2] @ acknowledge in REG_IF
	mov r1, #0x20C0 @ keep only GAMEPAK, SERIAL and TIMER3 enabled
	bic r2, r2, r0
	and r1, r1, r2
	strh r1, [r3] @ while the handler runs
	mrs r3, cpsr
	bic r3, r3, #0xDF
	orr r3, r3, #0x1F @ PSR_SYS_MODE
	msr cpsr_fc, r3
	ldr r1, =gIntrTable
	add r1, r1, ip
	ldr r0, [r1]
	stmdb sp!, {lr}
	adr lr, .Lirq_return
	bx r0
.Lirq_return:
	ldm sp!, {lr}
	mrs r3, cpsr
	bic r3, r3, #0xDF
	orr r3, r3, #0x92 @ PSR_IRQ_MODE, IRQs disabled
	msr cpsr_fc, r3
	pop {r0, r1, r2, r3, lr}
	strh r2, [r3]
	strh r1, [r3, #8]
	msr spsr_fc, r0
	bx lr
	.align 2, 0
	.pool

