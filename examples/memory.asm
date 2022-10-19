$define RAM_START 0x8000

lod r0, lo(RAM_START)
lod r1, hi(RAM_START)
lod r2, 0xf0

wtr A, r2
ldr r3, A

cmpi r3, 0xf0
jeqi addr(jmphere)
nop

jmphere:
	jmpi 0xffff
