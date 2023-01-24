entry:
	lad A, 0x8000
	lod r5, 0
loop:
	wtr A, r5

	cfg
	addi r0, 1
	adoi r1, 1
	
	cmpi r1, 0xff
	jnqi addr(loop)

	cmpi r0, 0xff
	jnqi addr(loop)

	jmpi 0xffff