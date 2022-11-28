$define PIN5 0b00001000

entry:
    lad A, 0x0
    lod r5, PIN5
loop:
    nori r5, 0
    nadi r5, PIN5 ; toggle pin 5
    nori r5, 0
    out A, r5


    lad B, addr(ret1)
    jmpi addr(delay)
ret1:
    lad B, addr(ret2)
    jmpi addr(delay)
ret2:
    lad B, addr(ret3)
    jmpi addr(delay)
ret3:
    lad B, addr(ret4)
    jmpi addr(delay)
ret4:
    lad B, addr(ret5)
    jmpi addr(delay)
ret5:
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
