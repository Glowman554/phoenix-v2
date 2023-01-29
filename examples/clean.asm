entry:
	lad A, 0x8000
	lod r5, 0
loop:
	wtr A, r5

	cfg
	add r0, 1
	ado r1, 1
	
	cmp r1, 0xff
	jnq loop

	cmp r0, 0xff
	jnq loop

	jmp 0xffff