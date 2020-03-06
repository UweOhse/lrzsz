#ifndef LRZSZ_H
#define LRZSZ_H
#include <unistd.h>
#include <time.h> // time_t
#include <sys/stat.h> // mode_t

struct lrzsz_fileinfo {
	char *fname;
	time_t modtime;
	mode_t mode;
	size_t bytes_total;
	size_t bytes_sent;
	size_t bytes_received;
	size_t bytes_skipped; /* crash recovery */
	int    eof_seen;
};

enum lrzsz_protocol_enum {
	LRZSZ_XMODEM,
	LRZSZ_YMODEM,
	LRZSZ_ZMODEM
};
struct lrzsz_io_config {
	int two_stopbits; // true: use two stop bits

	int may_use_stderr; // stderr is not connected to stdin or stdout.
};

struct lrzsz_config {
	struct lrzsz_io_config   io;

	enum lrzsz_protocol_enum protocol;
//	struct lrzsz_fileinfo  **files;
	struct lrzsz_fileinfo   *file;

	unsigned long            baudrate; // valid from 
};


#define LRZSZ_IOMODE_RESET                  0
#define LRZSZ_IOMODE_RAW                    1
#define LRZSZ_IOMODE_UNRAW_G                2
#define LRZSZ_IOMODE_RAW_WITH_FLOWCONTROL   3
int lrzsz_iomode(int fd, int mode, struct lrzsz_config *);
void lrzsz_check_stderr(struct lrzsz_config *);
unsigned long lrzsz_get_baudrate(unsigned long speedcode);

#endif
