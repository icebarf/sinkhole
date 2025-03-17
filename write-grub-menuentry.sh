#!/bin/sh
cat > ${ISODIR}/boot/grub/grub.cfg << EOF
menuentry "Sinkhole Operating System" {
	multiboot /boot/sinkhole.kernel
}
EOF