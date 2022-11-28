start:
	lod r0, 10

loop:
	sub r0, 1
	cmp r0, 0
	jnz loop

	jmp 0xffff
