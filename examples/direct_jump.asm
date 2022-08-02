start:
	lod r0, 10

loop:
	subi r0, 1
	cmpi r0, 0
	jnzi addr(loop)

	jmpi 0xffff