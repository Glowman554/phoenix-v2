TARGET=pico_emulator.elf

bash scripts/build.sh

echo "Goign to debug $TARGET"

gdb-multiarch -ex "target remote localhost:3333" -ex "load" -ex "monitor reset init" -ex "b main" -ex "continue" -ex "tui enable" build/$TARGET
