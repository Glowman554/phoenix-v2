entry:
    lad A, 0x8005

    lod r2, 1
    wtr A, r2

    add r0, 1
    lod r2, 4
    wtr A, r2

    add r0, 1
    lod r2, 8
    wtr A, r2

    add r0, 1
    lod r2, 10
    wtr A, r2

    add r0, 1
    lod r2, 11
    wtr A, r2

    add r0, 1
    lod r2, 2
    wtr A, r2

    add r0, 1
    lod r2, 5
    wtr A, r2

    lod r1, 0x80
    lod r2, 6 ; number of elements to loop over (-1 element)
    lod r3, 0 ; will be the result (should be 0x29)

loop:
    lod r0, 0x05
    add r0, r2
    ldr r4, A ; load from memory at offset

    add r3, r4 ; sum the values

    cmp r2, 0
    sub r2, 1
    jnz loop

done:
    jmp 0xffff
