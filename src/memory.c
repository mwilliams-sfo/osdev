
#include <stddef.h>

void * memset(void * buf, int c, size_t len) {
	char * p = buf;
	while (len > 0) {
		*p++ = c;
		len--;
	}
	return buf;
}
