	.section .header, "ax"
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
	.ascii "TEST"
RomHeaderMakerCode:
	.ascii "01"
RomHeaderMagic:
	.byte 0x96
RomHeaderMainUnitCode:
	.byte 0x00
RomHeaderDeviceType:
	.byte 0x00
RomHeaderReserved1:
	.space 7
RomHeaderSoftwareVersion:
	.byte 0x00
RomHeaderChecksum:
	.byte 0xF0
RomHeaderReserved2:
	.space 2
