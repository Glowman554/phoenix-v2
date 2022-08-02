entry:
	lod r0, 10
	lod r1, 20
	lod r2, lo(ret)
	lod r3, hi(ret)
	jmpi addr(subprocadd)
ret:
	jmpi 0xffff

subprocadd:
	add r0, r1
	jmp B