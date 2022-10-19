$define RAM_START 0x8005

entry:
    lod r0, lo(RAM_START)
    lod r1, hi(RAM_START)

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

    lod r1, hi(RAM_START)
    lod r2, 6 ; number of elements to loop over (-1 element)
    lod r3, 0 ; will be the result (should be 0x29)