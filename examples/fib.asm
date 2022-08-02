entry:
	lod r0, 0
	lod r1, 1
	lod r2, 12 ; compute the 13th fibonacci number leaves the result in r3

loop:
	mov r3, r0
	add r3, r1
	mov r0, r1
	mov r1, r3
	subi r2, 1
	cmpi r2, 0
	jnzi addr(loop)

done:
	jmpi 0xffff