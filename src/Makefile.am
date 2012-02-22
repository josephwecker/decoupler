MAKEFLAGS=--no-p -j
AM_CFLAGS = -I$(srcdir)/../lib -Wno-unused-parameter
bin_PROGRAMS = dcpl decoupler
dcpl_SOURCES = dcpl.c
decoupler_SOURCES = decoupler.c
dcpl_LDADD   = $(srcdir)/../lib/libdio.la
