dnl Process this file with autoconf to produce a configure script.
AC_INIT([GNU lrzsz],0.13.0-alpha, [<uwe@ohse.de>], [lrzsz], [https://ohse.de/uwe/software/lrzsz.html])
AC_CONFIG_SRCDIR([src/crctab.c])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)
AC_USE_SYSTEM_EXTENSIONS
LT_INIT

dnl AC_C_CROSS is in AC_PROG_CC since 2.12
AC_PREREQ(2.69)
AC_AUTOCONF_VERSION dnl just note the version of autoconf

PACKAGE_VERSION="$PACKAGE-$VERSION"
AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE", [Define to the name of the package])
AC_DEFINE_UNQUOTED(VERSION, "$VERSION", [Define to the version of the package.])
AC_DEFINE_UNQUOTED(PACKAGE_VERSION, "$PACKAGE_VERSION", [The concatenation of the package name, "-", and its version.])
AC_SUBST(PACKAGE)
AC_SUBST(VERSION)
AC_SUBST(PACKAGE_VERSION)

ALL_LINGUAS="de"

AC_SUBST(PACKAGE)
AC_SUBST(VERSION)

changequote(,)dnl
case $VERSION in
	[0-9]*.[0-9]*.[0-9]*) README_ALPHA="README-alpha";;
	*) README_ALPHA=;;
esac
changequote([, ])dnl


AC_ARG_ENABLE(timesync,
	[AS_HELP_STRING([--enable-timesync],[support the timesync protocol])],
	[case "$enableval" in 
		yes)
			AC_DEFINE(ENABLE_TIMESYNC,1,[Define to support the timesync protocol])
			;;
		no) ;;
		*) AC_MSG_ERROR([bad value $enableval for timesync option])
	esac]
	)
AC_SUBST(ENABLE_TIMESYNC)
AC_ARG_ENABLE(pubdir,
	[--enable-pubdir=/path   include support for a public writeable directory],
	[if test "$enableval" = "no" ; then 
		: 
	  else 
	    AC_DEFINE_UNQUOTED(PUBDIR,"$enableval")
	  fi])
AC_ARG_ENABLE(mkdir,
	[--disable-mkdir   disable support for creating directories (lrz)],
	[if test "$enableval" = "no" ; then 
		:
	  else
	  	AC_DEFINE(ENABLE_MKDIR)
	  fi],
	[AC_DEFINE(ENABLE_MKDIR)])
AC_ARG_ENABLE(syslog,
	[--enable-syslog=FACILITY,{force,default,optional} include syslogsupport],
	[
	  if test "$enableval" = "no" ; then 
		:
	  else
	    case "$enableval" in
	    *,*)
			level=`echo $enableval|sed -e 's/^.*,//'`
			fac=`echo $enableval|sed -e 's/,.*$//'`
			AC_DEFINE_UNQUOTED(ENABLE_SYSLOG,$fac)
			;;
		*)
			lookup_facility=LOG_UUCP
			level="$enableval"
			;;
		esac
	  	if test "$level" = "force" ; then
		  AC_DEFINE(ENABLE_SYSLOG_FORCE)
	  	else 
	  	  if test "$level" = "optional" ; then
	  	    :
	  	  else
		    AC_DEFINE(ENABLE_SYSLOG_DEFAULT)
		  fi
	  	fi
	  fi],[
		AC_DEFINE(ENABLE_SYSLOG_DEFAULT)
		lookup_facility=LOG_UUCP
	  ])

if test "x$CFLAGS" = x ; then old_cflags_empty=1; fi

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_RANLIB
if test "x$GCC" = xyes ; then
    if test "x$old_cflags_empty" = x1 ; then
        if test "x$_CFLAGS_OWG" != x  ; then
            # each environment should contain at least one helpful hack :-)
            CFLAGS="$_CFLAGS_OWG"
        fi
    fi
fi
AC_PROG_GCC_TRADITIONAL
AC_C_CONST
AC_C_INLINE

if test `hostname` = alpine.x9.ohse.de -a "$cross_compiling" = no ; then
    test $prefix = NONE && prefix=/usr
	if test -z "$CFLAGS" -o "x$CFLAGS" = "x-g -O2" ; then 
	    case "$CC" in 
		*gcc*)
			CFLAGS="-Wall -Wstrict-prototypes -Wmissing-prototypes"
			# -Wnested_externs entfernt wegen dcgettext()
			CFLAGS="$CFLAGS -Wpointer-arith -Wcast-qual -Wcast-align"
			CFLAGS="$CFLAGS -Winline -Wwrite-strings -Wshadow -Wmissing-braces"
			CFLAGS="$CFLAGS -Wcomments -fforce-addr -O2 -pipe"
			CFLAGS="$CFLAGS -falign-loops=2 -falign-jumps=2 -falign-functions=2 -g3"
			# CFLAGS="-Werror"
			;;
		esac
	fi
fi

dnl Checks for libraries.
dnl SCO needs this.
AC_CHECK_LIB(socket,syslog,,,)
AC_CHECK_LIB(be,syslog,,,)
dnl slowlartis needs this
AC_CHECK_LIB(nsl,gethostbyname,,,)
dnl
AC_CHECK_LIB(intl,gettext,,,)

dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_MAJOR
AC_CHECK_HEADERS(fcntl.h limits.h sys/ioctl.h sys/time.h unistd.h sys/times.h)
LRZSZ_HEADERS_TERM_IO
AC_CHECK_HEADERS(termios.h sys/termios.h termio.h sys/termio.h sgtty.h)
AC_CHECK_HEADERS(sys/mman.h utime.h syslog.h sys/syslog.h sys/param.h)
AC_CHECK_HEADERS(sys/select.h strings.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_UID_T
AC_TYPE_PID_T
LRZSZ_TYPE_SPEED_T
AC_CHECK_MEMBERS([struct stat.st_rdev])
AC_HEADER_TIME
lrzsz_HEADER_SYS_SELECT
AC_STRUCT_TM

dnl Checks for library functions.
AC_TYPE_SIGNAL
dnl suggested by autoscan, but not really needed.
dnl AC_FUNC_UTIME_NULL
AC_FUNC_MMAP
AC_FUNC_ALLOCA
AC_EGREP_HEADER([struct.*utimbuf], utime.h, AC_DEFINE(HAVE_STRUCT_UTIMBUF))

AC_CHECK_FUNCS(gettimeofday settimeofday)
AC_CHECK_FUNCS(memcpy select vprintf)
AC_CHECK_FUNCS(rdchk utime syslog siginterrupt)
AC_REPLACE_FUNCS(vasprintf)

AC_REPLACE_GNU_GETOPT

dnl for lib/error.c
AC_DEFINE(HAVE_STRERROR, 1, [we have a replacement function for this, but need to tell lib/error.c about it.])


dnl special tests
dnl
AC_MSG_CHECKING([for timezone variable])
AC_CACHE_VAL(libquark_cv_var_timezone,[
AC_TRY_LINK([#include <time.h>],[return(int)(timezone/2);],
	[libquark_cv_var_timezone=yes],
	[libquark_cv_var_timezone=no])
])
if test $libquark_cv_var_timezone = yes ; then
	AC_MSG_RESULT(yes)
	AC_DEFINE(HAVE_TIMEZONE_VAR)
else
	AC_MSG_RESULT(no)
fi
if test "x$lookup_facility" = x ; then
	:
else
	AC_MSG_CHECKING([for $lookup_facility])
	AC_CACHE_VAL(lrzsz_cv_lookup_facility,[
		AC_EGREP_CPP(ihave$lookup_facility, [
#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#else
#include <sys/syslog.h>
#endif
#ifdef $lookup_facility
ihave$lookup_facility
#endif
		], lrzsz_cv_lookup_facility=yes, lrzsz_cv_lookup_facility=no)
	])
	if test $lrzsz_cv_lookup_facility = yes ; then
		AC_MSG_RESULT(yes)
		AC_DEFINE_UNQUOTED(ENABLE_SYSLOG,$lookup_facility)
	else
		AC_DEFINE(ENABLE_SYSLOG,LOG_USER)
		AC_MSG_RESULT(no)
	fi
fi


AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(LIBS)

AM_GNU_GETTEXT(external)
AM_GNU_GETTEXT_VERSION

dnl AC_DEFINE_UNQUOTED(LOCALEDIR,"$prefix/$DATADIRNAME",[The name of the directory for locale stuff])

AC_OUTPUT([Makefile lib/Makefile \
man/Makefile po/Makefile.in src/Makefile \
src/lrzszbug],
[
test -z "$CONFIG_HEADERS" || echo timestamp > stamp-h])
