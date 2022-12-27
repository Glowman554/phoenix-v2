TARGET=pico_emulator.uf2
DEVICE=/dev/sda1

bash scripts/build.sh

echo "Goign to flash $TARGET to $DEVICE"

mkdir -p device
sudo mount $DEVICE device || exit 1
sudo cp -v build/$TARGET device/.
sudo umount device
