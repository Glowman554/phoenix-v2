lod r2, lo(jmphere)
lod r3, hi(jmphere)
jmp B ; jump to the address in r2:r3
nop

jmphere:
	lod r0, 0xff
	mov r1, r0
	jmp A ; jump to the address in r2:r3