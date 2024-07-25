// SPDX-FileCopyrightText: Menix-OS Team <info@menix-os.org>
// SPDX-License-Identifier: LGPL-2.1-or-later
//? cat utility for menix.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void cat_buffd(const char* filename);
void cat_stream(const char* filename);

int main(int argc, char* argv[])
{
	int32_t opt;
	int8_t strm = 0;

	if (argc == 1)
	{
usage:
		fprintf(
			stderr,
			"Usage: %s [-u] <FILE>\n\ncat - Read FILE to standard output.\n\nOptions:\n-h - Print this help text.\n-u - Print each byte without delay as soon as it is read.\n\n",
			argv[0]);
		return EXIT_FAILURE;
	}
	while ((opt = getopt(argc, argv, "u")) != -1)
	{
		switch (opt)
		{
			case 'u': strm = 1; break;
			case 'h': goto usage;
			default: goto usage;
		}
	}

	if (argc - optind < 1)
	{
		fprintf(stderr, "Invalid number of arguments.\n");
		return EXIT_FAILURE;
	}

	for (int i = optind; i < argc; ++i)
	{
		if (strm)
		{
			cat_stream(argv[i]);
		}
		else
		{
			cat_buffd(argv[i]);
		}
	}
	return EXIT_SUCCESS;
}

//! Read a given file and print each byte to stdout as it is read.
void cat_stream(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (!file)
	{
		perror("error: failed to open file!");
		return;
	}

	char ch;
	while ((ch = fgetc(file)) != EOF)
	{
		putchar(ch);
	}

	fclose(file);
}

//! Read file into 4K sized buffers and print them to stdout.
void cat_buffd(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (!file)
	{
		perror("error: failed to open file.");
		return;
	}

	const size_t BUFF_SIZE = 4096;
	char* buff = malloc(BUFF_SIZE);
	if (!buff)
	{
		perror("error: failed to allocate read buffer memory!");
		fclose(file);
		return;
	}

	size_t bytesRead;

	while ((bytesRead = fread(buff, 1, BUFF_SIZE, file)) > 0)
	{
		buff[bytesRead] = '\0';
		printf("%.*s", (int)bytesRead, buff);
	}

	free(buff);
	fclose(file);
}
