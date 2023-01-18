entry:
	lad A, 0x0
	lod r5, 0

loop:
	nor r5, 0 ; invert pins
	out A, r5
	jmp loop