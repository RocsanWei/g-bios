#pragma once

#include <list.h>

#define MAX_DENT_NAME_SIZE  256

// g-bios system standard directory
#define DEV_ROOT   "/dev"

typedef struct {
	const struct list_node *list, *iter;
	struct list_node hash_list;
} DIR;

struct dirent {
	struct list_node hash_node;
	char d_name[MAX_DENT_NAME_SIZE];
};

DIR *opendir(const char *name);

struct dirent *readdir(DIR *dir);

int closedir(DIR *dir);
