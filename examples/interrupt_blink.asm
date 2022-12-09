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
    lod r3, 0b00000001
    out A, r3

    lad A, 0x0
    lod r5, 0b00001000
loop:
    nor r5, 0
    nad r5, 0b00001000
    nor r5, 0
    out A, r5


    lad B, ret1
    jmp delay
ret1:
    lad B, ret2
    jmp delay
ret2:
    lad B, ret3
    jmp delay
ret3:
    lad B, ret4
    jmp delay
ret4:
    lad B, ret5
    jmp delay
ret5:
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

intr:
    jmp 0xffff ; if interrupt happens just halt
