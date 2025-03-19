DEFAULT_HOST:=$(shell ../buildutils/default-host.sh)
HOST?=$(DEFAULT_HOST)
HOSTARCH:=$(shell ../buildutils/target-triplet-to-host.sh $(HOST))

CFLAGS?=-O2 -g
CPPFLAGS?=
CXXFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -Wshadow -Wconversion
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -D__SINKHOLE_KERNEL_DO_SERIAL_IO_DEBUG -Iinclude 
CXXFLAGS:=$(CXXFLAGS) -fno-exceptions -fno-rtti
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lsinkc -lgcc

ARCHDIR=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
CXXFLAGS:=$(CXXFLAGS) $(KERNEL_ARCH_CXXFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS)\
io.o\
log.o\
kernel.o\

OBJS=\
$(ARCHDIR)/crti.o\
$(ARCHDIR)/crtbegin.o\
$(KERNEL_OBJS)\
$(ARCHDIR)/crtend.o\
$(ARCHDIR)/crtn.o\

LINK_LIST=\
$(LDFLAGS) \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

.PHONY: all clean install install-headers install-kernel project_kernel
.SUFFIXES: .o .c .cpp .S

project_kernel:=$(PROJECT).kernel

all: $(project_kernel)

$(project_kernel): $(OBJS) $(ARCHDIR)/linker.ld
	$(CXX) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot $@

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

.cpp.o:
	$(CXX) -MD -c $< -o $@ -std=c++17 $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -rf $(project_kernel)
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -Rf --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: $(project_kernel)
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp $(project_kernel) $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)
