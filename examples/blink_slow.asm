entry:
    lod r0, lo(0x0) ; io port 0
    lod r1, hi(0x0)
    lod r5, 0
loop:
    nadi r5, 0b00100000 ; toggle pin 5
    out A, r5


    lod r2, lo(ret1)
    lod r3, hi(ret1)
    jmpi addr(delay)
ret1:

    jmpi addr(loop)
    ; jmpi 0xffff

delay:
    lod r4, 0x10
delayloop1:
    subi r4, 1
    
    lod r3, 0xff
delayloop2:
    subi r3, 1
    cmpi r3, 0
    jnzi addr(delayloop2)

    cmpi r4, 0
    jnzi addr(delayloop1)

    jmp B
