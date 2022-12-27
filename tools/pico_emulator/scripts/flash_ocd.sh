TARGET=pico_emulator.elf

bash scripts/build.sh

echo "Goign to flash $TARGET using openocd"

openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg -c "program build/$TARGET verify reset exit"
