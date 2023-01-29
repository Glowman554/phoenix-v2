entry:
	lad A, 0xff01
	lod r4, 0x0f
	out A, r4 ; set prescaler to 0x0f

	lad A, 0xff02
	lod r4, 0xff
	out A, r4 ; set compare to 0xff

	lad A, 0xff03
	lod r4, 0b00010011
	out A, r4 ; enable timer + prescaler + toggle

loop:
	jmp loop
