
#include <stddef.h>
#include <stdint.h>
#include "config.h"
#include "heap.h"

#include "paging.h"

#define PAGE_DIRECTORY_SIZE (PAGE_SIZE / sizeof(struct page_directory_entry))
#define PAGE_TABLE_SIZE (PAGE_SIZE / sizeof(struct page_table_entry))

void paging_set_directory(struct page_directory_entry * directory);
void paging_enable();

struct paging_space * paging_create_space(uint8_t flags) {
	struct paging_space * space = heap_calloc(sizeof(struct paging_space));
	if (!space) return NULL;
	space->directory = heap_calloc(PAGE_SIZE);
	if (!space->directory) goto err;
	space->tables = heap_calloc(PAGE_DIRECTORY_SIZE * sizeof(struct page_table *));
	if (!space->tables) goto err;

	unsigned npage = 0;
	for (int i = 0; i < PAGE_DIRECTORY_SIZE; i++) {
		struct page_directory_entry * dir_entry = &space->directory[i];

		// Create an identity map for pages [npage, npage + 1024).
		struct page_table_entry * table = space->tables[i] = heap_calloc(PAGE_SIZE);
		if (!table) goto err;
		for (int j = 0; j < PAGE_TABLE_SIZE; j++) {
			struct page_table_entry * table_entry = &table[j];
			*(uint8_t *)table_entry = flags;
			table_entry->address = npage++;
		}
		*(uint8_t *) dir_entry = flags;
		dir_entry->address = (unsigned) table >> 12;
	}
	return space;

err:
	if (space) {
		if (space->tables) {
			for (int i = 0; i < PAGE_DIRECTORY_SIZE; i++) {
				heap_free(space->tables[i]);
			}
		}
		heap_free(space->directory);
		heap_free(space->tables);
		heap_free(space);
	}
	return NULL;
}

struct paging_space * paging_init(void) {
	struct paging_space * space = paging_create_space(7);
	if (!space) return NULL;
	paging_set_directory(space->directory);
	paging_enable();

	return space;
}

