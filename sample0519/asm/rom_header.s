	.section .header, "ax"
	.arm
	.align 2, 0

	.global Start
Start:
	b MultiBootEntry

	.global RomHeaderNintendoLogo
RomHeaderNintendoLogo:
	.incbin "../common/nintendo_logo.bin"

	@ Remaining target-specific AGB header fields (0xA0-0xBF).
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x30, 0x31, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00

@ The multiboot entry: the BIOS starts a program sent over the link cable
@ here, after the header. The bytes after the branch are the multiboot
@ header fields the BIOS fills in (boot mode, client number), then a second
@ entry point, the one used when the GameCube boots the GBA over JoyBus.
	.global MultiBootEntry
MultiBootEntry:
	b BootMain
	.space 0x1C

@ JoyBus boot: answer the GameCube with this program's game code, then
@ wait for it to send the same code back.
	.global JoyBusEntry
JoyBusEntry:
	ldr r3, reg_keyinput
	mov r1, #0
	strh r1, [r3, #0x10] @ REG_JOYCNT
	mov r1, #0xC0
	lsl r1, r1, #8
	strh r1, [r3, #4] @ REG_RCNT: JoyBus mode
.Lwait_send_clear:
	ldrh r1, [r3, #0x28] @ REG_JOYSTAT
	ands r1, r1, #8 @ JOYSTAT general flag
	bne .Lwait_send_clear
	ldr r2, game_code
	ldr r1, [r2]
	str r1, [r3, #0x24] @ REG_JOY_TRANS
.Lwait_recv:
	ldrh r0, [r3, #0x10]
	and r0, r0, #7
	cmp r0, #4 @ JOYCNT_RECV_COMPLETE
	bne .Lwait_recv
	strh r0, [r3, #0x10] @ acknowledge
.Lwait_send:
	ldrh r0, [r3, #0x10]
	and r0, r0, #7
	cmp r0, #2 @ JOYCNT_SEND_COMPLETE
	bne .Lwait_send
	strh r0, [r3, #0x10] @ acknowledge
	ldr r0, [r3, #0x20] @ REG_JOY_RECV
	cmp r0, r1
.Lwrong_code:
	bne .Lwrong_code @ not the code it was sent: hang

@ Clear RAM and the registers, then unpack the payload to EWRAM and run it.
BootMain:
	mov r0, #0x80 @ RESET_REGS
	svc #0x10000 @ RegisterRamReset
	mov r0, #0x40 @ RESET_SOUND_REGS
	svc #0x10000 @ RegisterRamReset
	ldr r0, payload_src
	ldr r1, payload_dest
	svc #0x110000 @ LZ77UnCompWram
	ldr lr, payload_dest
	bx lr

reg_keyinput:   .4byte 0x04000130 @ REG_KEYINPUT; the JoyBus registers follow
game_code:    .4byte 0x020000AC @ RomHeaderGameCode
payload_src:  .4byte Payload
payload_dest: .4byte 0x02018000
	.space 0x100
