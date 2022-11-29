entry:
    lad A, 0x0
    lod r5, 0b00100000
loop:
    nor r5, 0
    nad r5, 0b00100000 ; toggle pin 5
    nor r5, 0
    out A, r5


    lad B, ret1
    jmp delay
ret1:

    jmp loop
    ; jmpi 0xffff

delay:
    lod r4, 0x10
delayloop1:
    sub r4, 1
    
    lod r3, 0xff
delayloop2:
    sub r3, 1
    cmp r3, 0
    jnz delayloop2

    cmp r4, 0
    jnz delayloop1

    jmp B
