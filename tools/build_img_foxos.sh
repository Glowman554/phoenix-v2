PREFIX=phoenix-
DISK=disk
function copy_bin {
    echo "Copying $1/bin/$PREFIX$2 -> $DISK/BIN/$PREFIX$2.elf"
    cp $1/bin/$PREFIX$2 $DISK/BIN/$PREFIX$2.elf
}

bash build_foxos.sh

mkdir $DISK/BIN -p

copy_bin assembler as
copy_bin disassembler disasm
copy_bin emulator emu
copy_bin microcode microcode

cp ../examples $DISK -rv
rm $DISK/examples/Makefile $DISK/examples/tasks $DISK/examples/*gm2.* -rv

make -f /opt/foxos_sdk/disk.mak FOLDER=`pwd`/$DISK/*
