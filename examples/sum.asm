$define SUM_ARRAY_START 0x8005

entry:
    lad A, SUM_ARRAY_START

    lod r2, 1
    wtr A, r2

    addi r0, 1
    lod r2, 4
    wtr A, r2

    addi r0, 1
    lod r2, 8
    wtr A, r2

    addi r0, 1
    lod r2, 10
    wtr A, r2

    addi r0, 1
    lod r2, 11
    wtr A, r2

    addi r0, 1
    lod r2, 2
    wtr A, r2

    addi r0, 1
    lod r2, 5
    wtr A, r2

    lod r1, hi(SUM_ARRAY_START)
    lod r2, 6 ; number of elements to loop over (-1 element)
    lod r3, 0 ; will be the result (should be 0x29)

loop:
    lod r0, lo(SUM_ARRAY_START)
    add r0, r2
    ldr r4, A ; load from memory at offset

    add r3, r4 ; sum the values

    cmpi r2, 0
    subi r2, 1
    jnzi addr(loop)

done:
    jmpi 0xffff
