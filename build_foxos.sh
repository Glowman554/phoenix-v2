function build_dir {
	(
		cd $1
		make -f /opt/foxos_sdk/program.mak PROGRAM_NAME=$2 USER_CFLAGS="-Wno-write-strings -Wno-format-overflow" USER_CPPFLAGS="-Wno-write-strings -Wno-format-overflow"

		cp bin/$2 $3 -v 
	)
}

function build_file {
	(
		cd $1
		make -f /opt/foxos_sdk/program.mak PROGRAM_NAME=$2 USER_CFLAGS="-Wno-write-strings -Wno-format-overflow" CSRC=$3 USER_CPPFLAGS="-Wno-write-strings -Wno-format-overflow"

		cp bin/$2 $4 -v 
	)
}

mkdir disk/BIN -p
build_dir assembler ccpu-as.elf `pwd`/disk/BIN

build_file tools ccpu-disas.elf disassembler.c `pwd`/disk/BIN
build_file tools ccpu-emu.elf emulator.c `pwd`/disk/BIN

cp examples disk -rv
rm disk/examples/Makefile

make -f /opt/foxos_sdk/disk.mak FOLDER=`pwd`/disk/*