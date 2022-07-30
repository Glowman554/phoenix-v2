PREFIX=ccpu-

function install {
	echo "Installing $1"
	sudo cp -v $1 /bin/$PREFIX$2
}

make -C tools
make -C assembler

install tools/disassembler.elf disasm
install tools/emulator.elf emu
install tools/microcode.elf microcode
install assembler/as.elf as