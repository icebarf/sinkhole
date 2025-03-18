SYSTEM_HEADER_PROJECTS=libc kernel
PROJECTS=libc kernel

# Suffix above project names(dirs) with below suffixes
# using GNU Make's foreach
# Then use GNU Make's subst to remove suffix and run recipe
# This is done in order to prevent overriding of old targets
# DO NOT CHANGE THIS BEHAVIOR UNLESS YOU KNOW WHAT YOU ARE DOING
SYSPROJ_SUFFIX:=_sysproj_dir
PROJ_SUFFIX:=_proj_dir
CLEAN_SUFFIX:=_dir_c

export PROJECT:=sinkhole

include make.config

ISODIR:=$(shell pwd)/isodir

.PHONY: headers $(SYSTEM_HEADER_PROJECTS) $(PROJECTS) build iso launch clean

SYSPROJ:=$(foreach dir, $(SYSTEM_HEADER_PROJECTS),$(dir)$(SYSPROJ_SUFFIX))
headers: $(SYSPROJ)
$(SYSPROJ):
	mkdir -p "$(SYSROOT)"
	cd $(subst $(SYSPROJ_SUFFIX),,$@) && DESTDIR=$(SYSROOT) $(MAKE) install-headers

PROJ:=$(foreach dir, $(PROJECTS),$(dir)$(PROJ_SUFFIX))
build: headers $(PROJ)
$(PROJ):
	cd $(subst $(PROJ_SUFFIX),,$@) && DESTDIR=$(SYSROOT) $(MAKE) install

# NOTE: the `$(shell ...)` directive runs before everything else in the
# target recipe. Therefore, create the directory there even though
# we can put a recipe before it.
# For sanity, I will be moving the $(shell ...) directive as the first
# command of the target. This comment may seem outdated but it is actually
# a self note.
iso: build
	$(shell mkdir -p $(ISODIR)/boot/grub; ./buildutils/write-grub-menuentry.sh $(ISODIR)/boot/grub/grub.cfg)
	cp $(SYSROOT)/boot/$(PROJECT).kernel $(ISODIR)/boot/$(PROJECT).kernel
	grub-mkrescue -o $(PROJECT).iso $(ISODIR)

launch: iso
	qemu-system-$(shell ./buildutils/target-triplet-to-host.sh $(HOST)) -cdrom $(PROJECT).iso -serial file:serial.log

# Get each directory's name and put `_dir_c` behind it
CLEANING_AREA:=$(foreach dir, $(PROJECTS),$(dir)_dir_c)
clean: $(CLEANING_AREA) ROOT_CLEAN

$(CLEANING_AREA):
	cd $(subst _dir_c,,$@) && $(MAKE) clean

ROOT_CLEAN:
	rm -rf sysroot
	rm -rf isodir
	rm -rf sinkhole.iso
	rm -rf $(wildcard *.log)
