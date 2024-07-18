# SPDX-FileCopyrightText: Menix-OS Team <info@menix-os.org>
# SPDX-License-Identifier: LGPL-2.1-or-later
cmake_minimum_required(VERSION 3.25)

project(apps)

# include_directories(../mlibc/options/posix/include/)
# include_directories(../mlibc/options/ansi/include/)

# add_compile_options(-nostdinc)
add_subdirectory(src/cat)
add_subdirectory(src/echo)
# add_subdirectory(ls)
# add_subdirectory(mv)
