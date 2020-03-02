/*
  timing.c - Timing routines for computing elapsed wall time
  Copyright (C) 1994 Michael D. Black
  Copyright (C) 1996, 1997 Uwe Ohse

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
  02111-1307, USA.

  originally written by Michael D. Black, mblack@csihq.com
*/

#include "zglobal.h"

#include "timing.h"

#if HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#if !defined(TIME_WITH_SYS_TIME) && !defined(HAVE_SYS_TIME_H)
   /* can't use gettimeofday without struct timeval */
#  undef HAVE_GETTIMEOFDAY
#endif

/* Prefer gettimeofday to ftime to times.  */
#if defined(HAVE_GETTIMEOFDAY)
#  undef HAVE_FTIME
#  undef HAVE_TIMES
#else
#  if defined(HAVE_FTIME)
#    undef HAVE_TIMES
#  endif
#endif

#ifdef HAVE_FTIME
#  include <sys/timeb.h>
#endif

#ifdef HAVE_TIMES
#  if HAVE_SYS_TIMES_H
#    include <sys/times.h>
#  endif
#  ifdef _SC_CLK_TCK
#    define HAVE_SC_CLK_TCK 1
#  else
#    define HAVE_SC_CLK_TCK 0
#  endif
/* TIMES_TICK may have been set in policy.h, or we may be able to get
   it using sysconf.  If neither is the case, try to find a useful
   definition from the system header files.  */
#  if !defined(TIMES_TICK) && (!defined(HAVE_SYSCONF) || !defined(HAVE_SC_CLK_TCK))
#    ifdef CLK_TCK
#      define TIMES_TICK CLK_TCK
#    else /* ! defined (CLK_TCK) */
#      ifdef HZ
#        define TIMES_TICK HZ
#      endif /* defined (HZ) */
#    endif /* ! defined (CLK_TCK) */
#else
#  endif /* TIMES_TICK == 0 && (! HAVE_SYSCONF || ! HAVE_SC_CLK_TCK) */
#  ifndef TIMES_TICK
#    define TIMES_TICK 0
#  endif
#endif /* HAVE_TIMES */

double 
timing (int reset, time_t *nowp)
{
  static double starttime, stoptime;
  double yet;
#define NEED_TIME
#ifdef HAVE_GETTIMEOFDAY
  struct timeval tv;
  struct timezone tz;

#ifdef DST_NONE
  tz.tz_dsttime = DST_NONE;
#else
  tz.tz_dsttime = 0;
#endif
  gettimeofday (&tv, &tz);
  yet=tv.tv_sec + tv.tv_usec/1000000.0;
#undef NEED_TIME
#endif

#ifdef NEED_TIME
	yet=(double) time(NULL);
#endif
  if (nowp)
    *nowp=(time_t) yet;
  if (reset) {
    starttime = yet;
    return starttime;
  }
  else {
    double elaptime;
    stoptime = yet;
    elaptime = stoptime - starttime;
    return elaptime;
  }
}

/*#define TEST*/
#ifdef TEST
main()
{
	int i;
	printf("timing %g\n",timing(1,NULL));
	printf("timing %g\n",timing(0,NULL));
	for(i=0;i<20;i++){
	sleep(1);
	printf("timing %g\n",timing(0,NULL));
	}
}
#endif
