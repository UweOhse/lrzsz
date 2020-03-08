#include "config.h"

#include "zglobal.h"
#include "error.h"

#include <stdlib.h>
#include <string.h>

void *
xmalloc (size_t sz)
{
	void *p = malloc (sz);
	if (!p) {
		error(1,0,_("out of memory"));
	}
	return p;
}


char *
xstrdup (char const *str)
{
	size_t sz=strlen(str);
	char *t = xmalloc(sz);
	strcpy(t, str);
	return t;
}
