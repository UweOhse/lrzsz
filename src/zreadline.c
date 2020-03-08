/*
  zreadline.c - line reading stuff for lrzsz
  Copyright (C) until 1998 Chuck Forsberg (OMEN Technology Inc)
  Copyright (C) 1994 Matt Porter
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
/* once part of lrz.c, taken out to be useful to lsz.c too */

#include "zglobal.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>

#include "error.h"


/* Ward Christensen / CP/M parameters - Don't change these! */
#define TIMEOUT (-2)

static size_t readline_readnum;
static int readline_fd;
static char *readline_buffer;
int readline_left=0;
char *readline_ptr;

static void
zreadline_alarm_handler(int dummy)
{
	dummy++; /* doesn't need to do anything */
}

/*
 * This version of readline is reasonably well suited for
 * reading many characters.
 *
 * timeout is in tenths of seconds
 */
int 
readline_internal(unsigned int timeout)
{

	if (!no_timeout)
	{
		unsigned int n;
		n = timeout/10;
		if (n < 2 && timeout!=1)
			n = 3;
		else if (n==0)
			n=1;
		signal(SIGALRM, zreadline_alarm_handler); 
#ifdef HAVE_SIGINTERRUPT
		siginterrupt(SIGALRM,1);
#endif  
		alarm(n);
	}

	readline_ptr=readline_buffer;
	readline_left=read(readline_fd, readline_ptr, readline_readnum);
	if (!no_timeout)
		alarm(0);
	if (readline_left==-1)
		lrzsz_log(LOG_DEBUG,NULL,"read returned %s",strerror(errno));
	if (readline_left < 1)
		return TIMEOUT;
	--readline_left;
	return (*readline_ptr++ & 0377);
}



void
readline_setup(int fd, size_t readnum, size_t bufsize)
{
	readline_fd=fd;
	readline_readnum=readnum;
	readline_buffer=malloc(bufsize > readnum ? bufsize : readnum);
	if (!readline_buffer)
		error(1,0,_("out of memory"));
}

void
readline_purge(void)
{
	readline_left=0;
	return;
}

