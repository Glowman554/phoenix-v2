entry:
	lod r0, 10
	lod r1, 20
	lad B, addr(ret)
	jmpi addr(subprocadd)
ret:
	jmpi 0xffff

subprocadd:
	add r0, r1
	jmp B