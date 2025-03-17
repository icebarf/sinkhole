SYSTEM_HEADER_PROJECTS=libc kernel
PROJECTS=libc kernel

export PROJECT:=sinkhole

include make.config

ISODIR=isodir

.PHONY: headers $(SYSTEM_HEADER_PROJECTS) $(PROJECTS) build iso

headers: $(SYSTEM_HEADER_PROJECTS)
$(SYSTEM_HEADER_PROJECTS):
	mkdir -p "$(SYSROOT)"
	cd $@ && DESTDIR=$(SYSROOT) $(MAKE) install-headers

build: headers $(PROJECTS)
$(PROJECTS):
	cd $@ && DESTDIR=$(SYSROOT) $(MAKE) install

iso: build
	mkdir -p $(ISODIR)/boot/grub
	cp $(SYSROOT)/boot/$(PROJECT).kernel $(ISODIR)/boot/$(PROJECT).kernel
	$(shell ISODIR=$(ISODIR) ./write-grub-menuentry.sh)
	grub-mkrescue -o $(PROJECT).iso $(ISODIR)

launch: iso
	qemu-system-$(shell ./target-triplet-to-host.sh $(HOST)) -cdrom $(PROJECT).iso

