entry:
	lod r15, 0
	lad B, 0x0

	lspi 0xffff

	lihi addr(intr)

	lad A, 0xff00
	lod r4, 0b01000000
	out A, r4 ; unmask int6 (timer)

	lad A, 0xff01
	lod r4, 0x0f
	out A, r4 ; set prescaler to 0x0f

	lad A, 0xff02
	lod r4, 0x1f
	out A, r4 ; set compare to 0x0f

	lad A, 0xff03
	lod r4, 0b00000011
	out A, r4 ; enable timer + prescaler

loop:
	addi r0, 1
	addi r1, 2
	addi r14, 2
	jmpi addr(loop)

intr:
	put r15
	rfg r15

	put r0
	put r1
	put r14

	lad A, 0x8000
	ldr r14, A
	nori r14, 0
	wtr A, r14
	lad A, 0x0
	out A, r14

	pop r14
	pop r1
	pop r0

	wfg r15
	pop r15
	ire
