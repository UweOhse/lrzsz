/*
  check_stderr.c - helper function for lrzsz
*/
#include "zglobal.h"

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>

#ifdef MAJOR_IN_MKDEV
#include <sys/mkdev.h>
#else
# ifdef MAJOR_IN_SYSMACROS
# include <sys/sysmacros.h>
# endif
#endif

void
lrzsz_check_stderr(struct lrzsz_config *cfg)
{
	cfg->io.may_use_stderr = 0; // default
#ifdef HAVE_STRUCT_STAT_ST_RDEV
#  if defined(major) && defined(minor)
	struct stat st0, st1, st2;
	if (-1==fstat(0,&st0)) return;
	if (-1==fstat(1,&st1)) return;
	if (-1==fstat(2,&st2)) return;
	if (major(st0.st_rdev)!=major(st2.st_rdev)
	 || minor(st0.st_rdev)!=minor(st2.st_rdev)) {
		cfg->io.may_use_stderr = 1;
		return;
	 }
	if (major(st0.st_dev)!=major(st2.st_dev)
	 || minor(st0.st_dev)!=minor(st2.st_dev)) {
		cfg->io.may_use_stderr = 1;
		return;
	 }
#  endif
#endif
	return;
}

