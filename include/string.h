#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void * memset(void * buf, int c, size_t len);
size_t strlen(const char * s);
char * strncpy(char * dst, const char * src, size_t n);

#endif
