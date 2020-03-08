bin_PROGRAMS=lrz lsz
COMMON_SOURCES=iomode.c baudrate.c check_stderr.c utils.c
lrz_SOURCES=lrz.c timing.c zperr.c zreadline.c crctab.c rbsb.c zm.c protname.c lsyslog.c canit.c $(COMMON_SOURCES)
lsz_SOURCES=lsz.c timing.c zperr.c zreadline.c crctab.c rbsb.c zm.c protname.c lsyslog.c canit.c $(COMMON_SOURCES)

noinst_HEADERS = timing.h zglobal.h zmodem.h
datadir = @prefix@/share
localedir = @datadir@/locale
AM_CFLAGS=@CFLAGS@
DISTCLEAN_FILES=lrzszbug

LDADD = ../lib/libzmodem.a @LIBINTL@
EXTRA_DIST = lrzszbug.in
AM_CPPFLAGS = -I.. -I$(srcdir) -I$(top_srcdir)/src -I../intl -I$(top_srcdir)/lib
DEFS = @DEFS@ -DLOCALEDIR=\"$(localedir)\"

install-exec-local:
	rm -f $(DESTDIR)/$(bindir)/`echo lsb | sed -e '$(transform)'`
	ln $(DESTDIR)/$(bindir)/`echo lsz |sed -e '$(transform)'` \
		$(DESTDIR)/$(bindir)/`echo lsb |sed -e '$(transform)'` 
	rm -f $(bindir)/`echo lsx | sed -e '$(transform)'`
	ln $(DESTDIR)/$(bindir)/`echo lsz |sed -e '$(transform)'` \
		$(DESTDIR)/$(bindir)/`echo lsx |sed -e '$(transform)'` 
	rm -f $(bindir)/`echo lrb | sed -e '$(transform)'`
	ln $(DESTDIR)/$(bindir)/`echo lrz |sed -e '$(transform)'` \
		$(DESTDIR)/$(bindir)/`echo lrb |sed -e '$(transform)'` 
	rm -f $(bindir)/`echo lrx | sed -e '$(transform)'`
	ln $(DESTDIR)/$(bindir)/`echo lrz |sed -e '$(transform)'` \
		$(DESTDIR)/$(bindir)/`echo lrx |sed -e '$(transform)'` 
#	rm -f $(bindir)/`echo lrzszbug | sed -e '$(transform)'`
#	ln $(bindir)/`echo lrzszbug |sed -e '$(transform)'` \
#		$(bindir)/`echo lrzszbug |sed -e '$(transform)'` 

uninstall-local:
	rm -f $(DESTDIR)/$(bindir)/`echo lsb |sed -e '$(transform)'` 
	rm -f $(DESTDIR)/$(bindir)/`echo lsx |sed -e '$(transform)'` 
	rm -f $(DESTDIR)/$(bindir)/`echo lrb |sed -e '$(transform)'` 
	rm -f $(DESTDIR)/$(bindir)/`echo lrx |sed -e '$(transform)'` 
#	rm -f $(bindir)/`echo lrzszbug |sed -e '$(transform)'` 
