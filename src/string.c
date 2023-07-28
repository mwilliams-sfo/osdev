#include <stddef.h>

void * memset(void * buf, int c, size_t len) {
	char * p = buf;
	for (; len > 0; len--) {
		*p++ = c;
	}
	return buf;
}

void * memcpy(void * restrict dst, const void * restrict src, size_t len) {
	for (int i = 0; i < len; i++) {
		*((char *) dst + i) = *((const char *) src + i);
	}
	return dst;
}

size_t strlen(const char * s) {
	size_t n = 0;
	for (; *s; s++) n++;
	return n;
}

char * strncpy(char * dst, const char * src, size_t n) {
	int i;
	for (i = 0; i < n && src[i]; i++) {
		dst[i] = src[i];
	}
	for (; i < n; i++) {
		dst[i] = 0;
	}
	return dst;
}
