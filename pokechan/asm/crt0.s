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
@ ARM interrupt entry, as in pret/colosseum-mb.
@ Preserve the IRQ context, dispatch in system mode, then restore it.
IntrMain:
	mov r3, #0x4000000
	add r3, r3, #0x200
	ldr r2, [r3]
	ldrh r1, [r3, #8]
	mrs r0, spsr
	push {r0, r1, r2, r3, lr}
	mov r0, #1
	strh r0, [r3, #8]
	and r1, r2, r2, lsr #16
	mov ip, #0
	ands r0, r1, #0x80
	bne .Ldispatch_irq
	add ip, ip, #4
	ands r0, r1, #1
	bne .Ldispatch_irq
	add ip, ip, #4
	ands r0, r1, #2
	bne .Ldispatch_irq
	add ip, ip, #4
	ands r0, r1, #4
	bne .Ldispatch_irq
	add ip, ip, #4
	ands r0, r1, #8
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
	strbne r0, [r3, #-0x17c]
.Lgamepak_removed:
	bne .Lgamepak_removed
.Ldispatch_irq:
	strh r0, [r3, #2]
	mov r1, #0x20c0
	bic r2, r2, r0
	and r1, r1, r2
	strh r1, [r3]
	mrs r3, cpsr
	bic r3, r3, #0xdf
	orr r3, r3, #0x1f
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
	bic r3, r3, #0xdf
	orr r3, r3, #0x92
	msr cpsr_fc, r3
	pop {r0, r1, r2, r3, lr}
	strh r2, [r3]
	strh r1, [r3, #8]
	msr spsr_fc, r0
	bx lr
	.align 2, 0
	.pool
