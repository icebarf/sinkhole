#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir/boot/grub

cp sysroot/boot/sinkhole.kernel isodir/boot/sinkhole.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "Sinkhole Operating System" {
	multiboot /boot/sinkhole.kernel
}
EOF
grub-mkrescue -o sinkhole.iso isodir