/*
  lsyslog.c - wrapper for the syslog function
  Copyright (C) 1997 Uwe Ohse

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

*/
#include "config.h"
#include <stdarg.h>
#include "zglobal.h"
#ifdef ENABLE_SYSLOG
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#endif

#ifdef ENABLE_SYSLOG
static const char *username;

static void
lrzsz_syslog_init(void)
{
	static int init_done=0;
	static char uid_string[20]=""; /* i'd really hate this function to fail! */
	uid_t uid;
	struct passwd *pwd;

	if (init_done) {
		return;
	}
	init_done=1;

	uid=getuid();
	pwd=getpwuid(uid);
	if (pwd && pwd->pw_name && *pwd->pw_name) {
		username=strdup(pwd->pw_name);
	}
	if (!username) {
		username=uid_string;
		sprintf(uid_string,"#%lu",(unsigned long) uid);
	}
}

#endif

void
lrzsz_syslog(int prio, struct zm_fileinfo *fi, const char *format, ...)
{
        va_list ap;
	char *s=NULL;
	if (-1==prio) {
		return;
	}
#ifdef ENABLE_SYSLOG
	lrzsz_syslog_init();

	va_start(ap, format);
	vasprintf(&s,format, ap);
	va_end(ap);
	if (fi && fi->fname) {
		syslog(prio, "[%s] %s %s",username, fi->fname, s);
	} else {
		syslog(prio, "[%s] - %s",username, s);
	}
	free(s);
#else
	(void) format; /* get rid of warning */
	(void) fi; /* get rid of warning */
#endif
}

void
lsyslog(int prio, const char *format, ...)
{
	char *s=NULL;
        va_list ap;
	if (-1==prio) {
		return;
	}
#ifdef ENABLE_SYSLOG
	lrzsz_syslog_init();

	va_start(ap, format);
	vasprintf(&s,format, ap);
	va_end(ap);
	syslog(prio,"[%s] %s",username,s);
	free(s);
#else
	(void) format; /* get rid of warning */
#endif
}


int parse_syslog_facility(const char *s) {
#ifdef ENABLE_SYSLOG
	if (0==strcasecmp(s,"kern")) return LOG_KERN; // 0
	if (0==strcasecmp(s,"kernel")) return LOG_KERN;
	if (0==strcasecmp(s,"user")) return LOG_USER;     // 1
	if (0==strcasecmp(s,"mail")) return LOG_MAIL;     // 2
	if (0==strcasecmp(s,"daemon")) return LOG_DAEMON;   // 3
	if (0==strcasecmp(s,"auth")) return LOG_AUTH;     // 4
	if (0==strcasecmp(s,"syslog")) return LOG_SYSLOG;   // 5
	if (0==strcasecmp(s,"lpr")) return LOG_LPR;      // 6
	if (0==strcasecmp(s,"news")) return LOG_NEWS;     // 7
	if (0==strcasecmp(s,"uucp")) return LOG_UUCP;     // 8
	if (0==strcasecmp(s,"cron")) return LOG_CRON;     // 9
	if (0==strcasecmp(s,"authpriv")) return LOG_AUTHPRIV; // 10
	if (0==strcasecmp(s,"ftp")) return LOG_FTP;      // 11
	if (0==strcasecmp(s,"ntp")) return 12 << 3;
	if (0==strcasecmp(s,"audit")) return 13 << 3;
	// "alert") return 14<<3... this is just too confusing */
	if (0==strcasecmp(s,"clock")) return  15 << 3;
	if (0==strcasecmp(s,"local0")) return LOG_LOCAL0;
	if (0==strcasecmp(s,"local1")) return LOG_LOCAL1;
	if (0==strcasecmp(s,"local2")) return LOG_LOCAL2;
	if (0==strcasecmp(s,"local3")) return LOG_LOCAL3;
	if (0==strcasecmp(s,"local4")) return LOG_LOCAL4;
	if (0==strcasecmp(s,"local5")) return LOG_LOCAL5;
	if (0==strcasecmp(s,"local6")) return LOG_LOCAL6;
	if (0==strcasecmp(s,"local7")) return LOG_LOCAL7;
#endif
	return -1;
}
