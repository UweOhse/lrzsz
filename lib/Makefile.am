noinst_LIBRARIES=libzmodem.a
AM_CFLAGS=@CFLAGS@

EXTRA_DIST = alloca.c \
	getopt.c getopt1.c \
	vasprintf.c

libzmodem_a_SOURCES = long-options.c xstrtol.c xstrtoul.c error.c 
libzmodem_a_LIBADD  = @LIBOBJS@
libzmodem_a_DEPENDENCIES  = @LIBOBJS@

noinst_HEADERS = getopt.h long-options.h xstrtol.h xstrtoul.h error.h

