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

