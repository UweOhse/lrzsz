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

#include <sys/time.h>


double 
timing (int reset, time_t *nowp)
{
	static double starttime;
	double yet=0.0;
	int ok = 0;

#ifdef HAVE_GETTIMEOFDAY
	struct timeval tv;
	if (0==gettimeofday (&tv, NULL)) {
		yet=tv.tv_sec + tv.tv_usec/1000000.0;
		ok=1;
	}
#endif

	if (!ok) {
		yet=(double) time(NULL);
	}

	if (nowp)
		*nowp=(time_t) yet;
	if (reset) {
		starttime = yet;
		return starttime;
	} else {
		double elaptime;
		elaptime = yet - starttime;
		return elaptime;
	}
}
