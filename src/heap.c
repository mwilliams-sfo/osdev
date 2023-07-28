
#include <stddef.h>
#include <stdint.h>
#include "config.h"
#include "string.h"

#define HEAP_SIZE (100U << 20)
#define HEAP_BLOCK_SIZE PAGE_SIZE
#define HEAP_BLOCK_TYPE_FREE 0
#define HEAP_BLOCK_TYPE_USED 1
#define HEAP_TABLE_SIZE (HEAP_SIZE / HEAP_BLOCK_SIZE)
#define HEAP_BASE_ADDRESS 0x01000000U

struct heap_entry {
	uint8_t type: 4;
	uint8_t: 2;
	uint8_t has_next: 1;
	uint8_t first: 1;
} __attribute__((packed));

static struct heap_entry heap_table[HEAP_SIZE / HEAP_BLOCK_SIZE];
uint8_t (*heap_region)[HEAP_BLOCK_SIZE] = (uint8_t (*)[HEAP_BLOCK_SIZE]) HEAP_BASE_ADDRESS;

void * heap_alloc(unsigned size) {
	if (size == 0) return NULL;
	size += (~size + 1) & (HEAP_BLOCK_SIZE - 1);
	int n = size >> 12;

	// Find the necessary free blocks.
	int i = 0, j;
	for (j = 0; j < i + n; j++) {
		if (j == sizeof heap_table / sizeof heap_table[0]) return NULL;
		if (heap_table[j].type == HEAP_BLOCK_TYPE_USED) {
			i = j + 1;
		}
	}

	// Allocate the blocks.
	void * ptr = &heap_region[i];
	struct heap_entry * entry = &heap_table[i];
	entry->first = 1;
	for(; i < j; i++, entry++) {
		entry->type = HEAP_BLOCK_TYPE_USED;
		entry->has_next = 1;
	}
	(entry - 1)->has_next = 0;
	return (void *) ptr;
}

void * heap_calloc(unsigned size) {
	void * ptr = heap_alloc(size);
	if (!ptr) return NULL;
	return memset(ptr, 0, size);
}

void heap_free(void * ptr) {
	// Sanity-check the address.
	if (ptr < (void *) heap_region) return;
	if ((unsigned) ptr != ((unsigned) ptr & (HEAP_BLOCK_SIZE - 1))) return;
	int i = (unsigned) ptr >> 12;
	if (i >= sizeof heap_table / sizeof heap_table[0]) return;
	struct heap_entry * entry = &heap_table[i];
	if (!entry->first) return;

	// Unallocate blocks.
	while (1) {
		if (i == sizeof heap_table / sizeof heap_table[0]) return;
		if (!entry->has_next) break;
		*entry = (struct heap_entry) { 0 };
		i++; entry++;
	}
	*entry = (struct heap_entry) { 0 };
}
