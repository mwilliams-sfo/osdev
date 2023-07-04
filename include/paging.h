#ifndef PAGING_H
#define PAGING_H

#include "config.h"

struct page_directory_entry {
	unsigned present: 1;
	unsigned read_write: 1;
	unsigned user_supervisor: 1;
	unsigned write_through: 1;
	unsigned cache_disable: 1;
	unsigned accessed: 1;
	unsigned: 1;
	unsigned page_size: 1;
	unsigned: 4;
	unsigned address: 20;
} __attribute__((packed));

struct page_table_entry {
	unsigned present: 1;
	unsigned read_write: 1;
	unsigned user_supervisor: 1;
	unsigned write_through: 1;
	unsigned cache_disable: 1;
	unsigned accessed: 1;
	unsigned dirty: 1;
	unsigned page_attribute: 1;
	unsigned global: 1;
	unsigned: 3;
	unsigned address: 20;
} __attribute__((packed));

struct paging_space {
	struct page_directory_entry * directory;
	struct page_table_entry ** tables;
};

struct paging_space * paging_init(void);

#endif
