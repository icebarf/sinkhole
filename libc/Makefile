
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
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -Wshadow -Wconversion
CPPFLAGS:=$(CPPFLAGS) -D__is_libc -Iinclude
CXXFLAGS:=$(CXXFLAGS) -fno-exceptions -fno-rtti
LIBSINKC_CFLAGS:=$(CFLAGS)
LIBSINKC_CPPFLAGS:=$(CPPFLAGS) -D__is_libsinkc
LIBSINKC_CXXFLAGS:$(CXXFLAGS)

ARCHDIR=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
CXXFLAGS:=$(CXXFLAGS) $(KERNEL_ARCH_CXXFLAGS)
LIBSINKC_CFLAGS:=$(LIBSINKC_CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBSINKC_CPPFLAGS:=$(LIBSINKC_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LIBSINKC_CXXFLAGS:=$(LIBSINKC_CXXFLAGS) $(KERNEL_ARCH_CXXFLAGS)

FREEOBJS=\
$(ARCH_FREEOBJS) \
stdio/printf.o \
stdio/putchar.o \
stdio/puts.o \
stdlib/abort.o \
string/memcmp.o \
string/memcpy.o \
string/memmove.o \
string/memset.o \
string/strlen.o \
string/strncmp.o \
string/strncpy.o \

HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \

OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

LIBSINKC_OBJS=$(FREEOBJS:.o=.libsinkc.o)

#BINARIES=libc.a libsinkc.a
BINARIES=libsinkc.a

.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libsinkc.o .c .cpp .S

all: $(BINARIES)

libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

libsinkc.a: $(LIBSINKC_OBJS)
	$(AR) rcs $@ $(LIBSINKC_OBJS)

.cpp.o:
	echo "LIBC .cpp.o"
	$(CXX) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS)

.cpp.libsinkc.o:
	$(CXX) -MD -c $< -o $@ $(LIBSINKC_CFLAGS) $(LIBSINKC_CPPFLAGS) $(LIBSINKC_CXXFLAGS) -D__is_libsinkc

.S.libsinkc.o:
	$(CC) -MD -c $< -o $@ $(LIBSINKC_CFLAGS) $(LIBSINKC_CFLAGS) $(LIBSINKC_CPPFLAGS) $(LIBSINKC_CXXFLAGS)

clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBSINKC_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBSINKC_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-libs

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -Rf --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)

-include $(OBJS:.o=.d)
-include $(LIBSINKC_OBJS:.o=.d)