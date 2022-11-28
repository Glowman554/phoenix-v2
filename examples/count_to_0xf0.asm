entry:
    lad A, 0

    lod r2, 0

loop:
    add r2, 1
	cmp r2, 0xf0
    jnq loop

finish:
	jmp 0xffff
