entry:
    lod r0, lo(0x1) ; ddrb
    lod r1, hi(0x1)

    lod r2, 0b00100000 ; set PB5 as output
    out A, r2

    lod r0, lo(0x2) ; portb
    lod r1, hi(0x2)

loop:
    lod r2, 0b00000000 ; set PB5 low
    out A, r2

    lod r2, lo(ret1)
    lod r3, hi(ret1)
    jmpi addr(delay)
ret1:

    lod r2, 0b00100000 ; set PB5 high
    out A, r2


    lod r2, lo(ret2)
    lod r3, hi(ret2)
    jmpi addr(delay)
ret2:

    jmpi addr(loop)
    ; jmpi 0xffff

delay:
    lod r3, 10
delayloop:
    subi r3, 1
    cmpi r3, 0
    jnzi addr(delayloop)

    jmp B
