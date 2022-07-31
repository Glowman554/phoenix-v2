lod r2, lo(jmphere)
lod r3, hi(jmphere)
jmp B
nop

jmphere:
	lod r0, 0xff
	mov r1, r0
	jmp A