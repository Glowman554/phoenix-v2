entry:
    lod r0, lo(0x0) ; io port 0
    lod r1, hi(0x0)
    lod r5, 0
loop:
    nori r5, 0 ; invert pins
    out A, r5


    lod r2, lo(ret1)
    lod r3, hi(ret1)
    jmpi addr(delay)
ret1:

    jmpi addr(loop)
    ; jmpi 0xffff

delay:
    lod r3, 10
delayloop:
    subi r3, 1
    cmpi r3, 0
    jnzi addr(delayloop)

    jmp B
