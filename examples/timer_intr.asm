entry:
	lod r15, 0
	lad B, 0x0

	lihi addr(intr)

	lad A, 0xff00
	lod r4, 0b01000000
	out A, r4 ; unmask int6 (timer)

	lad A, 0xff01
	lod r4, 0x0f
	out A, r4 ; set prescaler to 0x0f

	lad A, 0xff02
	lod r4, 0x0f
	out A, r4 ; set compare to 0x0f

	lad A, 0xff03
	lod r4, 0b00000111
	out A, r4 ; enable timer + prescaler + intr

loop:
	jmpi addr(loop)

intr:
	nori r15, 0
	out B, r15
	ire
