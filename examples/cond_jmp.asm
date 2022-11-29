lod r2, 0x10
cmp r2, 0x10
jeq jmphere
nop

jmphere:
	jmp 0xffff
