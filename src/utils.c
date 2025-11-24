/*
 * Project: Simple Library Management System
 * File: utils.c
 * Author: Mobin Yousefi (GitHub: https://github.com/mobinyousefi-cs)
 * Created: 2025-11-24
 * Updated: 2025-11-24
 * License: MIT License (see LICENSE file for details)
 * -------------------------------------------------------------------------------------------------
 */

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void utils_clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* discard */
    }
}

int utils_read_int(const char *prompt, int min, int max) {
    int value;
    int result;

    for (;;) {
        printf("%s", prompt);
        result = scanf("%d", &value);
        if (result != 1) {
            printf("Invalid input. Please enter a number.\n");
            utils_clear_input_buffer();
            continue;
        }

        utils_clear_input_buffer();

        if (value < min || value > max) {
            printf("Please enter a value between %d and %d.\n", min, max);
            continue;
        }

        return value;
    }
}

void utils_read_line(const char *prompt, char *buffer, size_t size) {
    if (size == 0) {
        return;
    }

    printf("%s", prompt);

    if (fgets(buffer, (int)size, stdin) == NULL) {
        /* On error or EOF, set empty string. */
        buffer[0] = '\0';
        return;
    }

    /* Strip trailing newline if present. */
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

void utils_press_enter_to_continue(void) {
    printf("\nPress ENTER to continue...");
    fflush(stdout);

    int ch;
    /* If there is leftover input, flush until newline. */
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* discard */
    }
}
