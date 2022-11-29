entry:
	lod r0, 0
	lod r1, 1
	lod r2, 11 ; compute the 11th fibonacci number leaves the result in r3

loop:
	mov r3, r0
	add r3, r1
	mov r0, r1
	mov r1, r3
	sub r2, 1
	cmp r2, 0
	jnz loop

done:
    jmp 0xffff
