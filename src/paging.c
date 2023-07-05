
#include <stddef.h>
#include <stdint.h>
#include "config.h"
#include "heap.h"

#include "paging.h"

#define PAGE_DIRECTORY_SIZE (PAGE_SIZE / sizeof(page_directory_entry))
#define PAGE_TABLE_SIZE (PAGE_SIZE / sizeof(page_table_entry))

static struct paging_space * space;

void paging_set_directory(const page_directory_entry * directory);
void paging_enable();

static struct paging_space * paging_create_space(uint8_t flags) {
	struct paging_space * space = heap_calloc(sizeof(struct paging_space));
	if (!space) return NULL;
	space->directory = heap_calloc(PAGE_SIZE);
	if (!space->directory) goto err;
	space->tables = heap_calloc(PAGE_DIRECTORY_SIZE * sizeof(page_table_entry *));
	if (!space->tables) goto err;

	unsigned npage = 0;
	for (int i = 0; i < PAGE_DIRECTORY_SIZE; i++) {
		page_directory_entry * dir_entry = &space->directory[i];

		// Create an identity map for pages [npage, npage + 1024).
		page_table_entry * table = space->tables[i] = heap_calloc(PAGE_SIZE);
		if (!table) goto err;
		for (int j = 0; j < PAGE_TABLE_SIZE; j++) {
			table[j] = (npage++) << 12 | (flags & ~PAGE_TABLE_ADDRESS_MASK);
		}
		*dir_entry = ((unsigned) table & PAGE_TABLE_ADDRESS_MASK) | (flags & ~PAGE_TABLE_ADDRESS_MASK);
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

static int paging_find_page(void * addr, int * directory_index, int * table_index) {
	int page = (uint32_t) addr >> 12;
	*directory_index = page / PAGE_TABLE_SIZE;
	*table_index = page % PAGE_TABLE_SIZE;
	return 0;
}

int paging_init(void) {
	uint8_t flags = PAGE_TABLE_FLAGS_PRESENT | PAGE_TABLE_FLAGS_WRITABLE | PAGE_TABLE_FLAGS_USER;
	space = paging_create_space(flags);
	if (!space) return -1;
	paging_set_directory(space->directory);
	paging_enable();

	return 0;
}

int paging_map_address(void * vaddr, void * paddr) {
	int dir_index, table_index;
	int n = paging_find_page(vaddr, &dir_index, &table_index);
	if (n < 0) return n;
	if (!space) return -1;
	page_directory_entry dir_entry = space->directory[dir_index];
	page_table_entry * table = (page_table_entry * ) (dir_entry & PAGE_TABLE_ADDRESS_MASK);
	table[table_index] = ((page_table_entry) paddr & PAGE_TABLE_ADDRESS_MASK) |
		(table[table_index] & ~PAGE_TABLE_ADDRESS_MASK);
	return 0;
}
