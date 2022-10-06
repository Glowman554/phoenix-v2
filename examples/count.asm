entry:
    lod r0, lo(0x0)
    lod r1, hi(0x0)

    lod r2, 0

loop:
    addi r2, 1
    out A, r2
    jmpi addr(loop)
