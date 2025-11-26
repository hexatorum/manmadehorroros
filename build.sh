#!/usr/bin/env bash

echo "use the zig build system instead this is trash"
exit

CC=i686-elf-gcc
AS=i686-elf-as

base_no_ext() {
    local base=$(basename $1)
    echo "${base%%.*}"
}

mkdir -p out

$AS -o out/multiboot.o boot/multiboot.s
for file in src/*.c; do
    $CC -ffreestanding -std=c99 -c -o out/$(base_no_ext $file).o $file
done
$CC -T link.ld -ffreestanding -nostdlib -lgcc -o out/kernel.bin out/*.o

mkdir -p grub
mkdir -p grub/boot
mkdir -p grub/boot/grub

cp out/kernel.bin grub/boot
scripts/build-grub-config.sh $(basename out/kernel.bin) > grub/boot/grub/grub.cfg
grub-mkrescue -o out/os.iso grub