#ifndef FS_H
#define FS_H

struct path_part {
	char * name;
	struct path_part * next;
};

struct path {
	struct path_part * root;
};

void path_free(struct path * path);
struct path * path_parse(const char * s);

#endif
