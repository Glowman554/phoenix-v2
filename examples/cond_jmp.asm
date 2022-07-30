lod r0, lo(jmphere)
lod r1, hi(jmphere)
lod r2, 0x10
cmpi r2, 0x10
jeq A
nop

jmphere:
	lod r0, 0xff
	mov r1, r0
	jmp A