lod r0, lo(jmphere)
lod r1, hi(jmphere)
jmp A
nop

jmphere:
	lod r0, 0xff
	mov r1, r0
	jmp A