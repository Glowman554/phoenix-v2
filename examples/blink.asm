entry:
    lad A, 0x0
    lod r5, 0
loop:
    nor r5, 0
    out A, r5


    lad B, ret1
    jmp delay
ret1:

    jmp loop

delay:
    lod r3, 0x2
delayloop:
    sub r3, 1
    cmp r3, 0
    jnz delayloop

    jmp B
