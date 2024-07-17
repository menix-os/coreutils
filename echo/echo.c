// SPDX-FileCopyrightText: Menix-OS Team <info@menix-os.org>
// SPDX-License-Identifier: LGPL-2.1-only
//? echo utility for menix.

/*
 * echo.c - Print given input to the command line.
 *
 * Usage:
 *     ./echo [arguments, ...]
 *
 * Example:
 *     ./echo Hello, World!
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BIT(name, flag) (name & flag) == flag

// Flags
typedef enum
{
	TRAILING_NEWLINE = 1 << 0,
	BACKSLASH_ESCAPES = 1 << 1,
} flags;

int main(int argc, const char** argv)
{
	flags	flg = TRAILING_NEWLINE;
	ssize_t ret = 0;

	const char** arg_start = argv;

	for (int i = 1; i < argc; i++)
	{
		if (arg_start[i][0] == '-')
		{
			switch (arg_start[i][1])
			{
				// disable the trailing newline after the last argument by assigning the
				// bitflipped value
				case 'n':
					flg &= ~TRAILING_NEWLINE;
					argv++;
					argc--;
					break;
				case 'e':
					flg |= BACKSLASH_ESCAPES;
					argv++;
					argc--;
					break;
				default:
					break;	  // Invalid flags get treated as text.
			}
		}
	}

	for (int i = 1; i < argc; i++)
	{
		if (BIT(flg, BACKSLASH_ESCAPES))
		{
			for (int j = 0; j < strlen(argv[i]); j++)
			{
				if (argv[i][j] == '\\')
				{
					switch (argv[i][j + 1])
					{
						case 'a':
							write(1, "\a", 1);
							j++;
							break;	  // Write alert.
						case 'b':
							write(1, "\b", 1);
							j++;
							break;	  // Write backspace.
						case 'c':
							break;	  // Suppress teh newline that follows the last arg.
						case 'f':
							write(1, "\f", 1);
							j++;
							break;	  // Write form-feed.
						case 'n':
							write(1, "\n", 1);
							j++;
							break;	  // Write new-line.
						case 'r':
							write(1, "\r", 1);
							j++;
							break;
						case 't':
							write(1, "\t", 1);
							j++;
							break;
						case 'v':
							write(1, "\v", 1);
							j++;
							break;
						case '\\':
							write(1, "\\", 1);
							j++;
							break;
						default:
							break;
					}
				}
				else
				{
					ret = write(1, argv[i] + j, 1);
					if (ret == -1)
						goto fail;
				}
			}
		}
		else
		{
			// Write each argument to stdout.
			ret = write(1, argv[i], strlen(argv[i]));

			if (ret == -1)
			{
fail:
				perror("error: write syscall failed!");
				return 1;
			}
		}

		// Write a space after each argument except the last one.
		if (i < argc - 1)
		{
			write(1, " ", 1);
		}
	}

	// Write newline at the end.
	if (BIT(flg, TRAILING_NEWLINE))
	{
		write(1, "\n", 1);
	}

	return 0;
}
