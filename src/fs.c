
#include <stddef.h>
#include "string.h"
#include "heap.h"

#include "fs.h"

void path_free(struct path * path) {
	if (!path) return;
	for (struct path_part * part = path->root; part;) {
		struct path_part * next = part->next;
		heap_free(part->name);
		heap_free(part);
		part = next;
	}
	heap_free(path);
}

struct path * path_parse(const char * s) {
	if (!s || *s != '/') return NULL;

	struct path * path = heap_calloc(sizeof(struct path));
	if (!path) goto err;
	if (!s[1]) return path;

	struct path_part * part = path->root = heap_calloc(sizeof(struct path_part));
	while (1) {
		if (!part) goto err;

		const char * part_end = ++s;
		while (*part_end && *part_end != '/') {
			part_end++;
		}
		if (part_end == s) goto err;

		part->name = heap_calloc(part_end - s + 1);
		if (!part->name) goto err;
		strncpy(part->name, s, part_end - s);

		s = part_end;
		if (!*s) break;

		part = part->next = heap_calloc(sizeof(struct path_part));
	}
	return path;

err:
	path_free(path);
	return NULL;
}
