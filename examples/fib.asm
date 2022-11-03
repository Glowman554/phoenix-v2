entry:
	lod r0, 0
	lod r1, 1
	lod r2, 11 ; compute the 11th fibonacci number leaves the result in r3

loop:
	mov r3, r0
	add r3, r1
	mov r0, r1
	mov r1, r3
	subi r2, 1
	cmpi r2, 0
	jnzi addr(loop)

done:
loop2:
    lad A, 0x0
    out A, r3
    
    lod r4, 0
    out A, r4
    jmpi addr(loop2)

    jmpi 0xffff
