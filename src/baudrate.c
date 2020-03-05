#include "zglobal.h"
#include "lrzsz.h"

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#ifdef MAJOR_IN_MKDEV
#include <sys/mkdev.h>
#else
# ifdef MAJOR_IN_SYSMACROS
# include <sys/sysmacros.h>
# endif
#endif

static struct {
	unsigned baudr;
	speed_t speedcode;
} speeds[] = {
	{110,	B110},
	{300,	B300},
	{600,	B600},
	{1200,	B1200},
	{2400,	B2400},
	{4800,	B4800},
	{9600,	B9600},
#ifdef B19200
    {19200,  B19200},
#endif
#ifdef B38400
    {38400,  B38400},
#endif
#ifdef B57600
    {57600,  B57600},
#endif
#ifdef B115200
    {115200,  B115200},
#endif
#ifdef B230400
    {230400,  B230400},
#endif
#ifdef B460800
    {460800,  B460800},
#endif
#ifdef EXTA
	{19200,	EXTA},
#endif
#ifdef EXTB
	{38400,	EXTB},
#endif
	{0, 0}
};

unsigned long
lrzsz_get_baudrate(unsigned long code)
{
	int n;
	for (n=0; speeds[n].baudr; ++n)
		if (speeds[n].speedcode == (unsigned long)code)
			return speeds[n].baudr;
	return 38400;	/* Assume fifo if ioctl failed */
}
