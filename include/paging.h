#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include "config.h"

#define PAGE_TABLE_FLAGS_PRESENT 1
#define PAGE_TABLE_FLAGS_WRITABLE 2
#define PAGE_TABLE_FLAGS_USER 4
#define PAGE_TABLE_FLAGS_WRITE_THROUGH 8
#define PAGE_TABLE_FLAGS_CACHE_DISABLE 0x10
#define PAGE_TABLE_FLAGS_ACCESSED 0x20
#define PAGE_TABLE_FLAGS_DIRTY 0x40
#define PAGE_TABLE_ADDRESS_MASK (~(PAGE_SIZE - 1))

typedef uint32_t page_directory_entry;
typedef uint32_t page_table_entry;

struct paging_space {
	page_directory_entry * directory;
	page_table_entry ** tables;
};

int paging_init(void);
int paging_map_address(void * vaddr, void * paddr);

#endif
