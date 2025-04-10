SYSTEM_HEADER_PROJECTS:=libc kernel
PROJECTS:=libc kernel
MAIN_PROJECTS:= kernel
SIDE_PROJECTS:= libc

# Suffix above project names(dirs) with below suffixes
# using GNU Make's foreach
# Then use GNU Make's subst to remove suffix and run recipe
# This is done in order to prevent overriding of old targets
# DO NOT CHANGE THIS BEHAVIOR UNLESS YOU KNOW WHAT YOU ARE DOING
SYSHEADERPROJ_SUFFIX:=_sysproj_dir
PROJ_SUFFIX:=_proj_dir
CLEAN_SUFFIX:=_dir_c

export PROJECT:=sinkhole

include make.config

ISODIR:=$(shell pwd)/isodir

.PHONY: headers $(SYSTEM_HEADER_PROJECTS) $(PROJECTS) build iso launch clean

SYSHEADERPROJ:=$(foreach dir, $(SYSTEM_HEADER_PROJECTS),$(dir)$(SYSHEADERPROJ_SUFFIX))
headers: $(SYSHEADERPROJ)
$(SYSHEADERPROJ):
	mkdir -p "$(SYSROOT)"
	cd $(subst $(SYSHEADERPROJ_SUFFIX),,$@) && DESTDIR=$(SYSROOT) $(MAKE) install-headers


SIDE:=$(foreach dir, $(SIDE_PROJECTS),$(dir)$(PROJ_SUFFIX))
$(SIDE): $(SYSHEADERPROJ)
	cd $(subst $(PROJ_SUFFIX),,$@) && DESTDIR=$(SYSROOT) $(MAKE) install

MAIN:=$(foreach dir, $(MAIN_PROJECTS),$(dir)$(PROJ_SUFFIX))
$(MAIN): $(SIDE)
	cd $(subst $(PROJ_SUFFIX),,$@) && DESTDIR=$(SYSROOT) $(MAKE) install

PROJ:=$(MAIN)

# NOTE: the `$(shell ...)` directive runs before everything else in the
# target recipe. Therefore, create the directory there even though
# we can put a recipe before it.
# For sanity, I will be moving the $(shell ...) directive as the first
# command of the target. This comment may seem outdated but it is actually
# a self note.
# iso: build
iso: $(PROJ)
	$(shell mkdir -p $(ISODIR)/boot/grub; ./buildutils/write-grub-menuentry.sh $(ISODIR)/boot/grub/grub.cfg)
	cp $(SYSROOT)/boot/$(PROJECT).kernel $(ISODIR)/boot/$(PROJECT).kernel
	grub-mkrescue -o $(PROJECT).iso $(ISODIR)

launch: iso
	qemu-system-$(shell ./buildutils/target-triplet-to-host.sh $(HOST)) -cdrom $(PROJECT).iso -serial file:serial.log -cpu EPYC -m 512

# Get each directory's name and put `_dir_c` behind it
CLEANING_AREA:=$(foreach dir, $(PROJECTS),$(dir)_dir_c)
clean: $(CLEANING_AREA) ROOT_CLEAN

$(CLEANING_AREA):
	cd $(subst _dir_c,,$@) && $(MAKE) clean

ROOT_CLEAN:
	rm -rf .cache
	rm -rf sysroot
	rm -rf isodir
	rm -rf sinkhole.iso
	rm -rf $(wildcard *.log)
