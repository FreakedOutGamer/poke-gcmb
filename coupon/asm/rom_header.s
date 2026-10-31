	.section .header, "ax"
	.arm
	.align 2, 0

	.global Start
Start:
	b Init

	@ The Nintendo logo field is left blank in this image.
	.global RomHeaderNintendoLogo
RomHeaderNintendoLogo:
	.space 0x9C

RomHeaderGameTitle:
	.space 12
	.global RomHeaderGameCode
RomHeaderGameCode:
	.ascii "TEST"
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
