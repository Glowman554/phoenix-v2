entry:
	lad A, 0xff02
	lod r4, 0x0f
	out A, r4 ; set compare to 0x0f

	lad A, 0xff03
	lod r4, 0b00010001
	out A, r4 ; enable timer + toggle

loop:
	jmp loop
