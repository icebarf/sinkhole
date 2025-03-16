#!/bin/sh
set -e
. ./iso.sh
qemu-system-$(./target-triplet-to-host.sh $HOST) -cdrom sinkhole.iso