// SPDX-FileCopyrightText: Menix-OS Team <info@menix-os.org>
// SPDX-License-Identifier: LGPL-2.1-or-later
//? ls utility for menix.

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void list_dir(const char* path);

int main(int argc, char* argv[])
{
	const char *path;

	if (argc > 2)
	{
		fprintf(stderr, "Usage: %s [directory]\n", argv[0]);
		return EXIT_FAILURE;
	} else if (argc ==1) {
		path = ".";
	} else
	{
		path = argv[1];
	}

	list_dir(path);

	return EXIT_SUCCESS;
}

// List contents of a given path recursively.
void list_dir(const char* path)
{
	DIR *dir;
	struct dirent* entry;

	dir = opendir(path);
	if (dir == NULL)
	{
		fprintf(stderr, "error: unable to access '%s': %s\n", path, strerror(errno));
		return;
	}

	while ((entry = readdir(dir)) != NULL)
	{
		// Skip '.' and '..'
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		{
			continue;
		}

		printf("%s\n", entry->d_name);
	}

	if (closedir(dir) == -1)
	{
		perror("error: failure to close dirstream.");
	}
}
