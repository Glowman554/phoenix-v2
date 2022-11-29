lad A, 0x8000
lod r2, 0xf0

wtr A, r2
ldr r3, A

cmp r3, 0xf0
jeq jmphere
nop

jmphere:
	jmp 0xffff
