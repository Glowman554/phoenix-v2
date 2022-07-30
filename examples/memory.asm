lod r0, lo(0x8000)
lod r1, hi(0x8000)
lod r2, 0xf0

wtr A, r2
ldr r2, A

lod r0, lo(jmphere)
lod r1, hi(jmphere)
cmpi r2, 0xf0
jeq A
nop

jmphere:
	lod r0, lo(0xffff)
	lod r1, hi(0xffff)
	jmp A