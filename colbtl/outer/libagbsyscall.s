@ From colosseum-mb (asm/libagbsyscall.s).
	.text
	.syntax unified

	.align 2, 0
	.global RegisterRamReset
	.thumb
	.thumb_func
	.type RegisterRamReset, function
RegisterRamReset: @ 0x0200FD24
	svc #1
	bx lr
