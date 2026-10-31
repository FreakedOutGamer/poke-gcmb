	.section .raw, "ax"
	.align 2, 0

	@ payload.lz is found through the version's -I path (build/<version>/).
	@ The payload, relinked from the sources and recompressed. colbtl's
	@ bootstrap (outer/iwram.s) moves Payload..PayloadEnd out of the way.
	.global Payload
Payload:
	.incbin "payload.lz"
	.global PayloadEnd
PayloadEnd:
