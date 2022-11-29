entry:
    lad A, 0

    lod r2, 0

loop:
    add r2, 1
    out A, r2
    jmp loop
