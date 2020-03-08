#include <fcntl.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/types.h>
#include <utime.h>
#include <unistd.h>
#include <stdlib.h>
#include "zglobal.h"

/* logic:
	a) use fchmod()
	1. use futimens() if possible (POSIX.1-2008).
	2. use futimes() if possible (BSD).
	b) use chmod
	3. use utime() if possible (POSIX.1-2001. marked obsolete in POSIX.1-2008)

*/
int
close_and_update_meta(FILE *f, const char *name, time_t remote_modtime, mode_t mode)
{
	int ok_time = 0;
	int ok_mode = 0;
	int ret;
	int fd;
#ifdef HAVE_FUTIMENS
	struct timespec ts[2];
#endif
#ifdef HAVE_STRUCT_UTIMBUF
	struct utimbuf timep;
#endif
#ifdef HAVE_FUTIMES
	struct timeval tv[2];
#endif

	/* 
		we need to flush the buffers so that mtime isn't overwritten later.
		but that also means fflush can fail..
	 */

	ret = fflush(f);
	if (ret) {
		fclose(f);
		return ret;
	}

	fd=fileno(f);

#ifdef HAVE_FCHMOD
	if (0==fchmod(fd,mode))
		ok_mode=1;
#endif

#ifdef HAVE_FUTIMENS
	ts[0].tv_sec = remote_modtime;
	ts[0].tv_nsec = 500*1000*1000;
	ts[1].tv_sec = remote_modtime;
	ts[1].tv_nsec = 500*1000*1000;
	if (0==futimens(fd,ts))
		ok_time=1;
#endif
#ifdef HAVE_FUTIMES
	tv[0].tv_sec=remote_modtime;
	tv[0].tv_usec=500*1000;
	tv[1].tv_sec=remote_modtime;
	tv[1].tv_usec=500*1000;
	if (!ok_time && 0==futimes(fd,tv))
		ok_time=1;
#endif

	ret = fclose(f);
	if (ret)
		return ret;

#ifdef HAVE_STRUCT_UTIMBUF
	if (!ok_time) {
		timep.actime = time(NULL);
		timep.modtime = remote_modtime;
		utime(name, &timep);
	}
#endif
	if (!ok_mode) {
		// i think this will never be used. fchmod is quite old.
		chmod(name,mode);
	}
	return 0;
}
