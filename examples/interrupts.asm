entry:
	lih intr

	lad A, 0xff00
	lod r3, 0b10000000
	out A, r3 ; unmask int7

	int

	jmp 0xffff

intr:
	ire
