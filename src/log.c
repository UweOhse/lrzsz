/*
  log.c - lrzsz log handling
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
#include "error.h"

#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

static const char *username;
static int lrzsz_locallog_severity=LOG_INFO;
static int lrzsz_syslog_facility=LOG_UUCP;
static int lrzsz_syslog_severity=LOG_EMERG;
static int lrzsz_openlog_done;

static const char *severityname(int sev) {
	switch(sev) {
	case 0: return "Emergency";
	case 1: return "Alert";
	case 2: return "Critical";
	case 3: return "Error";
	case 4: return "Warning";
	case 5: return "Notice";
	case 6: return "Info";
	case 7: return "Debug";
	}
	return "Debug";
}

static void
lrzsz_log_init(void)
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
		username=xstrdup(pwd->pw_name);
	}
	if (!username) {
		username=uid_string;
		sprintf(uid_string,"#%lu",(unsigned long) uid);
	}
}

#define LRZSZ_LOG_BUF_SIZE 1024
void
lrzsz_log(int prio, struct zm_fileinfo *fi, const char *format, ...)
{
        va_list ap;

	char buf[LRZSZ_LOG_BUF_SIZE];
	int is_progress=0; // is this some kind of fast repeating process report?
	if (prio & L_PROGRESS) {
		is_progress=1;
		prio=prio^L_PROGRESS;
	}

	if (-1==prio) {
		return;
	}

	lrzsz_log_init();

	va_start(ap, format);
	vsnprintf(buf,LRZSZ_LOG_BUF_SIZE, format, ap);
	va_end(ap);

	if (prio<=lrzsz_locallog_severity) {
		// <level> filename formatted-message
		fprintf(stderr,"%s: %s %s\n", severityname(prio), 
			fi && fi->fname && *fi->fname ? fi->fname : "-",
			buf);
		fflush(stderr);
	}
	if (prio<=lrzsz_syslog_severity && !is_progress) {
		if (!lrzsz_openlog_done) {
			openlog(program_name, LOG_PID|LOG_NDELAY, lrzsz_syslog_facility);
			lrzsz_openlog_done=1;
		}
		syslog(prio, "[%s] %s %s",username, 
			fi && fi->fname && *fi->fname ? fi->fname : "-",
			buf);
	}
}


static int lrzsz_parse_syslog_severity(const char *s) {
	if (0==strcasecmp(s,"emerg")) return LOG_EMERG; // 0
	if (0==strcasecmp(s,"alert")) return LOG_ALERT; // 1
	if (0==strcasecmp(s,"crit")) return LOG_CRIT;     // 2
	if (0==strcasecmp(s,"err")) return LOG_ERR;     // 3
	if (0==strcasecmp(s,"warning")) return LOG_WARNING;   // 4
	if (0==strcasecmp(s,"notice")) return LOG_NOTICE;     // 5
	if (0==strcasecmp(s,"info")) return LOG_INFO;   // 6
	if (0==strcasecmp(s,"debug")) return LOG_DEBUG;      // 7
	return -1;
}
static int lrzsz_parse_syslog_facility(const char *s) {
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
	return -1;
}
void lrzsz_set_syslog_facility(const char *s) {
	int f;
	f=lrzsz_parse_syslog_facility(s);
	if (-1==f) {
		error(1,0,"invalid syslog facility %s",s);
	}
	lrzsz_syslog_facility=f;
}
void lrzsz_set_syslog_severity(const char *s) {
	int f;
	f=lrzsz_parse_syslog_severity(s);
	if (-1==f) {
		error(1,0,"invalid syslog severity %s",s);
	}
	lrzsz_syslog_severity=f;
}
void lrzsz_set_locallog_severity(const char *s) {
	int f;
	f=lrzsz_parse_syslog_severity(s);
	if (-1==f) {
		error(1,0,"invalid syslog severity %s",s);
	}
	lrzsz_locallog_severity=f;
}
