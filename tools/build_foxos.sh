PREFIX=phoenix-
function build_tool {
    echo "Building $1 as $PREFIX$2"
    (
        cd $1
        make -f /opt/foxos_sdk/program.mak USER_CFLAGS=-I../common PROGRAM_NAME=$PREFIX$2
    )
}

build_tool assembler as
build_tool disassembler disasm
build_tool emulator emu
build_tool microcode microcode

