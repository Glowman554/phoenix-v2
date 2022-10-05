entry:
    lod r0, hi(0x8005)
    lod r1, lo(0x8005)

    lod r2, 1
    wtr A, r2

    addi r1, 1
    lod r2, 4
    wtr A, r2

    addi r1, 1
    lod r2, 8
    wtr A, r2

    addi r1, 1
    lod r2, 10
    wtr A, r2

    addi r1, 1
    lod r2, 11
    wtr A, r2

    addi r1, 1
    lod r2, 2
    wtr A, r2

    addi r1, 1
    lod r2, 5
    wtr A, r2

    lod r0, hi(0x8005)
    lod r2, 6 ; number of elements to loop over (-1 element)
    lod r3, 0 ; will be the result (should be 0x29)

loop:
    lod r1, lo(0x8005)
    add r1, r2
    ldr r2, A ; load from memory at offset

    add r3, r2 ; sum the values
    
    mov r2, r1
    subi r2, lo(0x8005) ; reconstruct r2  from r1

    cmpi r2, 0
    subi r2, 1
    jnzi addr(loop)

done:
    jmpi 0xffff
