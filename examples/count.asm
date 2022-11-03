entry:
    lad A, 0

    lod r2, 0

loop:
    addi r2, 1
    out A, r2
    jmpi addr(loop)
