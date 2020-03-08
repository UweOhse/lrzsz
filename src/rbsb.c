/*
  rbsb.c - terminal handling stuff for lrzsz
  Copyright (C) until 1988 Chuck Forsberg (Omen Technology INC)
  Copyright (C) 1994 Matt Porter, Michael D. Black
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

  originally written by Chuck Forsberg
*/

/*
 *  Rev 05-05-1988
 *  ============== (not quite, but originated there :-). -- uwe 
 */
#include "zglobal.h"

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#ifdef READCHECK_FIONREAD
/*
 *  Return non 0 if something to read from io descriptor f
 */
int 
rdchk(int fd)
{
	static long lf;

	ioctl(fd, FIONREAD, &lf);
	return ((int) lf);
}
#endif

#ifdef READCHECK_GETFL
unsigned char checked = '\0' ;
/*
 * Nonblocking I/O is a bit different in System V, Release 2
 */
int 
rdchk(int fd)
{
	int lf, savestat;

	savestat = fcntl(fd, F_GETFL) ;
	if (savestat == -1)
		return 0;
#ifdef OVERLY_PARANOID
	if (-1==fcntl(fd, F_SETFL, savestat | O_NDELAY))
		return 0;
	lf = read(fd, &checked, 1) ;
	if (-1==fcntl(fd, F_SETFL, savestat)) {
#ifdef ENABLE_SYSLOG
		if (enable_syslog)
			lsyslog(LOG_CRIT,"F_SETFL failed in rdchk(): %s",	
				strerror(errno));
#endif
		zpfatal("rdchk: F_SETFL failed\n"); /* lose */
		/* there is really no way to recover. And we can't tell
		 * the other side what's going on if we can't write to
		 * fd, but we try.
		 */
		canit(fd);
		exit(1); 
	}
#else
	fcntl(fd, F_SETFL, savestat | O_NDELAY);
	lf = read(fd, &checked, 1) ;
	fcntl(fd, F_SETFL, savestat);
#endif
	return(lf == -1 && errno==EWOULDBLOCK ? 0 : lf) ;
}
#endif



void
sendbrk(int fd)
{
#ifdef HAVE_TCSENDBREAK
	tcsendbreak(fd,0);
#elif defined(TCSBRK)
	ioctl(fd, TCSBRK, 0);
#endif
}

void
purgeline(int fd)
{
	readline_purge();
#ifdef TCFLSH
	ioctl(fd, TCFLSH, 0);
#else
	lseek(fd, 0L, 2);
#endif
}

/* End of rbsb.c */
