
#include <stddef.h>
#include <stdint.h>

#define HEAP_SIZE (100U << 20)
#define HEAP_BLOCK_SIZE 4096U
#define HEAP_BLOCK_FLAG_FIRST 0x80
#define HEAP_BLOCK_FLAG_HASN 0x40
#define HEAP_BLOCK_TYPE_MASK 0xf
#define HEAP_BLOCK_TYPE_FREE 0
#define HEAP_BLOCK_TYPE_USED 1
#define HEAP_TABLE_SIZE (HEAP_SIZE / HEAP_BLOCK_SIZE)
#define HEAP_BASE_ADDRESS 0x01000000U

static uint8_t heap_table[HEAP_SIZE / HEAP_BLOCK_SIZE];
uint8_t (*heap_region)[HEAP_BLOCK_SIZE] = (uint8_t (*)[HEAP_BLOCK_SIZE]) HEAP_BASE_ADDRESS;

void * heap_alloc(unsigned size) {
	int n, i, j;
	void * ptr;

	if (size == 0) return NULL;
	size += (~size + 1) & (HEAP_BLOCK_SIZE - 1);
	n = size >> 12;

	// Find the necessary free blocks.
	i = 0;
	for (j = 0; j < i + n; j++) {
		if (j == sizeof heap_table / sizeof heap_table[0]) return NULL;
		if ((heap_table[j] & HEAP_BLOCK_TYPE_MASK) == HEAP_BLOCK_TYPE_USED) {
			i = j + 1;
		}
	}

	// Allocate the blocks.
	ptr = &heap_region[i];
	heap_table[i] = HEAP_BLOCK_FLAG_FIRST;
	for(; i < j; i++) {
		heap_table[i] |= HEAP_BLOCK_FLAG_HASN | HEAP_BLOCK_TYPE_USED;
	}
	heap_table[i - 1] &= ~HEAP_BLOCK_FLAG_HASN;
	return (void *) ptr;
}

void heap_free(void * ptr) {
	int i;

	if (ptr < (void *) heap_region) return;
	if ((unsigned) ptr != ((unsigned) ptr & (HEAP_BLOCK_SIZE - 1))) return;
	i = (unsigned) ptr >> 12;
	if (i >= sizeof heap_table / sizeof heap_table[0]) return;
	if ((heap_table[i] & HEAP_BLOCK_FLAG_FIRST) == 0) return;

	while (1) {
		if (i == sizeof heap_table / sizeof heap_table[0]) return;
		if ((heap_table[i] & HEAP_BLOCK_FLAG_HASN) == 0) break;
		heap_table[i++] = 0;
	}
	heap_table[i] = 0;
}
