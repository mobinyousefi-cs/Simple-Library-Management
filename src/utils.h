/*
 * Project: Simple Library Management System
 * File: utils.h
 * Author: Mobin Yousefi (GitHub: https://github.com/mobinyousefi-cs)
 * Created: 2025-11-24
 * Updated: 2025-11-24
 * License: MIT License (see LICENSE file for details)
 * -------------------------------------------------------------------------------------------------
 */

#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

/* Read an integer in [min, max] with prompt and basic validation. */
int  utils_read_int(const char *prompt, int min, int max);

/* Read a line of text (including spaces), strip trailing newline. */
void utils_read_line(const char *prompt, char *buffer, size_t size);

/* Wait for user to press ENTER (useful after menu actions). */
void utils_press_enter_to_continue(void);

#endif /* UTILS_H */
