#
# TODO:
#   http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/Vector-Extensions.html#Vector-Extensions
#   https://developer.apple.com/hardwaredrivers/ve/sse.html

# Determine the root directory for the project. The following is an alternate
# version if realpath isn't defined on your system:
# ROOTDIR=$(dir $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))
ROOTDIR    ?= $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
MFLAGS      = -k --no-p -j4
QUIET       = yes

# Guess about the build target if nothing's specified directly.
TARGET     ?= $(shell $(ROOTDIR)build/target)

# General compilation flags.
CFLAGS     += -Wall -Iinclude -pipe -combine

# Optimization flags. Feel free to move these into a target subsection if they
# aren't working on your target.
CFLAGS     += -O3                              \
							-finline-limit=20000             \
							-fwhole-program                  \
							-funroll-loops                   \
							-fvariable-expansion-in-unroller \
							-ftracer                         \
							-fivopts                         \
							-fsee                            \
							-ftree-vectorize                 \
							-ffast-math                      \
							-frename-registers               \
							-maccumulate-outgoing-args
#							-mregparm=3                      \
#							-msseregparm                     
ifeq ($(TARGET),Darwin)
	# TODO: get cpu stuff from system_profiler on osx
	CFLAGS   += -arch x86_64 -fast -march=core2 -mtune=core2 -msse2
	CFLAGS   += -DUSE_KQUEUE
else
ifeq ($(TARGET),Linux)
	# TODO: -Ofast if and only if gcc supports it
	CFLAGS   += -march=native -mtune=native -m64 -msse4
	CFLAGS   += -DUSE_EPOLL -DUSE_LINUX_SENDFILE -DUSE_SPLICE
endif # Linux
endif # Darwin


LDFLAGS    += 

all: dcpl

dcpl: Makefile src/dcpl.c
	$(CC) $(CFLAGS) src/dcpl.c -o $@

clean:
	rm -f dcpl
