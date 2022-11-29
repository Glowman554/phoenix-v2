entry:
    lad A, 0

    lod r2, 0

loop:
    addi r2, 1
	cmpi r2, 0xf0
    jnqi addr(loop)

finish:
	jmpi 0xffff
