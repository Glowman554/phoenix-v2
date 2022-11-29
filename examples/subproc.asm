entry:
	lod r0, 10
	lod r1, 20
	lad B, ret
	jmp subprocadd
ret:
	jmp 0xffff

subprocadd:
	add r0, r1
	jmp B
