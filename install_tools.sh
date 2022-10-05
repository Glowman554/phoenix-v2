PREFIX=ccpu-

function install {
	echo "Installing $1"
	sudo cp -v $1 /bin/$PREFIX$2
}

function build {
	echo "Building $1"
	make -C $1 clean -i
	make -C $1
}

build tools/assembler
build tools/disassembler
build tools/emulator
build tools/microcode
build tools/visual_eumlator

install tools/assembler/assembler.elf as
install tools/disassembler/disassembler.elf disasm
install tools/emulator/emulator.elf emu
install tools/microcode/microcode.elf microcode
install tools/visual_eumlator/visual_eumlator.elf emu-visual
