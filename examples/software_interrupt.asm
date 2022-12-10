entry:
    lad A, 0x8000 ; write jmp 0xxxxx to interrupt address
    lod r3, 0x1d
    wtr A, r3

    lad B, intr ; write address for jmp
    lad A, 0x8001
    wtr A, r2

    lad A, 0x8002
    wtr A, r3

    lad A, 0xff00
    lod r3, 0b10000000 ; unmask int7 (software interrupt)
    out A, r3

    lad A, 0x0
    lod r5, 0
loop:
	int
    jmp loop

intr:
    nor r5, 0
    out A, r5
	ire
