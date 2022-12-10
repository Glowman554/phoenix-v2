entry:
    lad A, 0x8000
    lod r3, 0x1d
    wtr A, r3

    lad B, intr
    lad A, 0x8001
    wtr A, r2

    lad A, 0x8002
    wtr A, r3

    lad A, 0xff00
    lod r3, 0b00000011
    out A, r3

loop:
	jmp loop

intr:
	lad A, 0xff00
	inp r3, A

	cmp r3, 0b00000001
	jeq led_on
	cmp r3, 0b00000010
	jeq led_off
	ire

led_on:
	lad A, 0
	lod r3, 0b00001000
	out A, r3
	ire

led_off:
	lad A, 0
	lod r3, 0b00000000
	out A, r3
	ire