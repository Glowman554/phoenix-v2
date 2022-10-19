$include(test2.asm)

$define HALT 0xffff

loop:
    lod r0, lo(RAM_START)
    add r0, r2
    ldr r4, A ; load from memory at offset

    add r3, r4 ; sum the values

    cmpi r2, 0
    subi r2, 1
    jnzi addr(loop)

done:
    jmpi HALT
