entry:
    lad A, 0x0
    lod r5, 0
loop:
    nori r5, 0 ; invert pins
    out A, r5


    lad B, addr(ret1)
    jmpi addr(delay)
ret1:

    jmpi addr(loop)
    ; jmpi 0xffff

delay:
    lod r3, 0x2
delayloop:
    subi r3, 1
    cmpi r3, 0
    jnzi addr(delayloop)

    jmp B
