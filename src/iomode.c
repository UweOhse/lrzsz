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

#include "zglobal.h"
#include "lrzsz.h"

#include <sys/types.h>

static struct termios oldtty;


/*
 * mode(n)
 *  3: save old tty stat, set raw mode with flow control
 *  2: set XON/XOFF for sb/sz with ZMODEM or YMODEM-g
 *  1: save old tty stat, set raw mode 
 *  0: restore original tty mode
 */
int 
lrzsz_iomode(int fd, int n, struct lrzsz_config *cf)
{
	static int did0 = FALSE;
	struct termios tty;

	lrzsz_syslog(LOG_DEBUG,NULL,"iomode: %d",n);

	switch(n) {

	case LRZSZ_IOMODE_UNRAW_G: /* Un-raw mode used by sz, sb when -g detected */
		if(!did0) {
			did0 = TRUE;
			tcgetattr(fd,&oldtty);
		}
		tty = oldtty;

		tty.c_iflag = BRKINT|IXON;

		tty.c_oflag = 0;	/* Transparent output */

		tty.c_cflag &= ~PARENB;	/* Disable parity */
		tty.c_cflag |= CS8;	/* Set character size = 8 */
		if (cf->io.two_stopbits)
			tty.c_cflag |= CSTOPB;	/* Set two stop bits */

#ifdef READCHECK
		tty.c_lflag = protocol==ZM_ZMODEM ? 0 : ISIG;
		tty.c_cc[VINTR] = protocol==ZM_ZMODEM ? -1 : 030;	/* Interrupt char */
#else
		tty.c_lflag = 0;
		tty.c_cc[VINTR] = protocol==ZM_ZMODEM ? 03 : 030;	/* Interrupt char */
#endif
#ifdef _POSIX_VDISABLE
		if (((int) _POSIX_VDISABLE)!=(-1)) {
			tty.c_cc[VQUIT] = _POSIX_VDISABLE;		/* Quit char */
		} else {
			tty.c_cc[VQUIT] = -1;			/* Quit char */
		}
#else
		tty.c_cc[VQUIT] = -1;			/* Quit char */
#endif
		tty.c_cc[VMIN] = 1;	 /* This many chars satisfies reads */
		tty.c_cc[VTIME] = 1;	/* or in this many tenths of seconds */

		tcsetattr(fd,TCSADRAIN,&tty);

		return OK;
	case LRZSZ_IOMODE_RAW:
	case LRZSZ_IOMODE_RAW_WITH_FLOWCONTROL:
		if(!did0) {
			did0 = TRUE;
			tcgetattr(fd,&oldtty);
		}
		tty = oldtty;

		tty.c_iflag = IGNBRK;
		if (n==LRZSZ_IOMODE_RAW_WITH_FLOWCONTROL)
			tty.c_iflag |= IXOFF;

		/* Setup raw mode: no echo, noncanonical (no edit chars),
		 * no signal generating chars, and no extended chars (^V, 
		 * ^O, ^R, ^W).
		 */
		tty.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
		tty.c_oflag = 0;	/* Transparent output */

		tty.c_cflag &= ~(PARENB);	/* Same baud rate, disable parity */
		/* Set character size = 8 */
		tty.c_cflag &= ~(CSIZE);
		tty.c_cflag |= CS8;	
		if (cf->io.two_stopbits)
			tty.c_cflag |= CSTOPB;	/* Set two stop bits */
		tty.c_cc[VMIN] = 1; /* This many chars satisfies reads */
		tty.c_cc[VTIME] = 1;	/* or in this many tenths of seconds */
		tcsetattr(fd,TCSADRAIN,&tty);
		cf->baudrate = lrzsz_get_baudrate(cfgetospeed(&tty));
		return OK;
	case LRZSZ_IOMODE_RESET:
		if(!did0)
			return ERROR;
		tcdrain (fd); /* wait until everything is sent */
		tcflush (fd,TCIOFLUSH); /* flush input queue */
		tcsetattr (fd,TCSADRAIN,&oldtty);
		tcflow (fd,TCOON); /* restart output */

		return OK;
	default:
		return ERROR;
	}
}

