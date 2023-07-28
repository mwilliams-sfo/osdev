#ifndef OS_FS_H
#define OS_FS_H

struct path_part {
	char * name;
	struct path_part * next;
};

struct path {
	struct path_part * root;
};

void path_destroy(struct path * path);
struct path * path_parse(const char * s);

#endif
