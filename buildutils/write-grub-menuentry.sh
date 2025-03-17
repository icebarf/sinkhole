#!/bin/sh
cat > $1 << EOF
menuentry "Sinkhole Operating System" {
	multiboot /boot/sinkhole.kernel
}
EOF