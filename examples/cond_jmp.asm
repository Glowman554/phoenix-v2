lod r2, 0x10
cmpi r2, 0x10
jeqi addr(jmphere)
nop

jmphere:
	jmpi 0xffff
