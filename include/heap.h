#ifndef OS_HEAP_H
#define OS_HEAP_H

void * heap_alloc(unsigned size);
void * heap_calloc(unsigned size);
void heap_free(void * ptr);

#endif
