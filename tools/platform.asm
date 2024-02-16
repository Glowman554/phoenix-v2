#bankdef memory
{
    #addr 0x00
    #size 0xffff
    #bits 8
    #outp 0
}
#bank memory

#ruledef
{
    nop                         => 0x00`8 @ 0b0000 @ 0b0000 @ 0b00000000
    mov r{reg1}, r{reg2}        => 0x01`8 @ reg2`4 @ reg1`4 @ 0b00000000
    lod r{reg1}, {value: u8}    => 0x02`8 @ 0b0000 @ reg1`4 @ value`8
    out A, r{reg1}              => 0x03`8 @ 0b0000 @ reg1`4 @ 0b00000000
    inp r{reg1}, A              => 0x04`8 @ 0b0000 @ reg1`4 @ 0b00000000
    jnz A                       => 0x05`5 @ 0b0000 @ 0b0000 @ 0b00000000

    add r{reg1}, r{reg2}        => 0x06`8 @ reg2`4 @ reg1`4 @ 0b00000000
    add r{reg1}, {value: u8}    => 0x07`8 @ 0b0000 @ reg1`4 @ value`8

    sub r{reg1}, r{reg2}        => 0x08`8 @ reg2`4 @ reg1`4 @ 0b00000000
    sub r{reg1}, {value: u8}    => 0x09`8 @ 0b0000 @ reg1`4 @ value`8

    nad r{reg1}, r{reg2}        => 0x0a`8 @ reg2`4 @ reg1`4 @ 0b00000000
    nad r{reg1}, {value: u8}    => 0x0b`8 @ 0b0000 @ reg1`4 @ value`8

    nor r{reg1}, r{reg2}        => 0x0c`8 @ reg2`4 @ reg1`4 @ 0b00000000
    nor r{reg1}, {value: u8}    => 0x0d`8 @ 0b0000 @ reg1`4 @ value`8

    cmp r{reg1}, r{reg2}        => 0x0e`8 @ reg2`4 @ reg1`4 @ 0b00000000
    cmp r{reg1}, {value: u8}    => 0x0f`8 @ 0b0000 @ reg1`4 @ value`8

    jzr A                       => 0x10`5 @ 0b0000 @ 0b0000 @ 0b00000000

    ldr r{reg1}, A              => 0x11`8 @ 0b0000 @ reg1`4 @ 0b00000000
    wtr A, r{reg1}              => 0x12`8 @ 0b0000 @ reg1`4 @ 0b00000000

    jmp A                       => 0x13`5 @ 0b0000 @ 0b0000 @ 0b00000000
    jeq A                       => 0x14`5 @ 0b0000 @ 0b0000 @ 0b00000000
    jnq A                       => 0x15`5 @ 0b0000 @ 0b0000 @ 0b00000000

    jnz B                       => 0x16`5 @ 0b0000 @ 0b0000 @ 0b00000000
    jzr B                       => 0x17`5 @ 0b0000 @ 0b0000 @ 0b00000000
    jmp B                       => 0x18`5 @ 0b0000 @ 0b0000 @ 0b00000000
    jeq B                       => 0x19`5 @ 0b0000 @ 0b0000 @ 0b00000000
    jnq B                       => 0x1a`5 @ 0b0000 @ 0b0000 @ 0b00000000

    ldr r{reg1}, B              => 0x1b`8 @ 0b0000 @ reg1`4 @ 0b00000000
    wtr B, r{reg1}              => 0x1c`8 @ 0b0000 @ reg1`4 @ 0b00000000

    jmp {value: u16}            => 0x1d`8 @ value[7:0] @ value[15:8]
    jeq {value: u16}            => 0x1e`8 @ value[7:0] @ value[15:8]
    jnq {value: u16}            => 0x1f`8 @ value[7:0] @ value[15:8]
    jzr {value: u16}            => 0x20`8 @ value[7:0] @ value[15:8]
    jnz {value: u16}            => 0x21`8 @ value[7:0] @ value[15:8]

    lad A, {value: u16}         => 0x22`8 @ value[7:0] @ value[15:8]
    lad B, {value: u16}         => 0x23`8 @ value[7:0] @ value[15:8]


    out B, r{reg1}              => 0x24`8 @ 0b0000 @ reg1`4 @ 0b00000000
    inp r{reg1}, B              => 0x25`8 @ 0b0000 @ reg1`4 @ 0b00000000

    ire                         => 0x26`8 @ 0b0000 @ 0b0000 @ 0b00000000
    int                         => 0x27`8 @ 0b0000 @ 0b0000 @ 0b00000000

    jof A                       => 0x28`5 @ 0b0000 @ 0b0000 @ 0b00000000
    jof B                       => 0x29`5 @ 0b0000 @ 0b0000 @ 0b00000000
    jof {value: u16}            => 0x2a`8 @ value[7:0] @ value[15:8]

    jno A                       => 0x2b`5 @ 0b0000 @ 0b0000 @ 0b00000000
    jno B                       => 0x2c`5 @ 0b0000 @ 0b0000 @ 0b00000000
    jno {value: u16}            => 0x2d`8 @ value[7:0] @ value[15:8]


    ado r{reg1}, r{reg2}        => 0x2e`8 @ reg2`4 @ reg1`4 @ 0b00000000
    ado r{reg1}, {value: u8}    => 0x2f`8 @ 0b0000 @ reg1`4 @ value`8

    sbo r{reg1}, r{reg2}        => 0x30`8 @ reg2`4 @ reg1`4 @ 0b00000000
    sbo r{reg1}, {value: u8}    => 0x31`8 @ 0b0000 @ reg1`4 @ value`8

    cfg                         => 0x32`8 @ 0b0000 @ 0b0000 @ 0b00000000

    lih A                       => 0x33`5 @ 0b0000 @ 0b0000 @ 0b00000000
    lih B                       => 0x34`5 @ 0b0000 @ 0b0000 @ 0b00000000
    lih {value: u16}            => 0x35`8 @ value[7:0] @ value[15:8]

    wfg r{reg1}                 => 0x36`8 @ 0b0000 @ reg1`4 @ 0b00000000
    rfg r{reg1}                 => 0x37`8 @ 0b0000 @ reg1`4 @ 0b00000000

    lsp A                       => 0x38`5 @ 0b0000 @ 0b0000 @ 0b00000000
    lsp B                       => 0x39`5 @ 0b0000 @ 0b0000 @ 0b00000000
    lsp {value: u16}            => 0x3a`8 @ value[7:0] @ value[15:8]

    rsp A                       => 0x3b`5 @ 0b0000 @ 0b0000 @ 0b00000000
    rsp B                       => 0x3c`5 @ 0b0000 @ 0b0000 @ 0b00000000



    put {value: u8}             => 0x3d`8 @ 0b0000 @ 0b0000 @ value`8
    put r{reg1}                 => 0x3e`8 @ 0b0000 @ reg1`4 @ 0b00000000

    pop r{reg1}                 => 0x3f`8 @ 0b0000 @ reg1`4 @ 0b00000000
}