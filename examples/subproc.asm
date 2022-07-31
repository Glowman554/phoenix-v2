entry:
	lod r0, lo(subproc)
	lod r1, hi(subproc)
	lod r2, lo(ret)
	lod r3, hi(ret)
	jmp A
ret:
	lod r0, lo(0xffff)
	lod r1, hi(0xffff)
	jmp A

subproc:
	lod r0, 10
	lod r1, 11
	add r0, r1
	jmp B