entry:
	lad A, 0x8000 ; put ire at interrupt entry point
	lod r3, 0x26
	wtr A, r3

	lad A, 0xff00
	lod r3, 0b00000001
	out A, r3 ; unmask int0

loop:
	nop
	jmp loop