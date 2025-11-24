/*
 * Project: Simple Library Management System
 * File: main.c
 * Author: Mobin Yousefi (GitHub: https://github.com/mobinyousefi-cs)
 * Created: 2025-11-24
 * Updated: 2025-11-24
 * License: MIT License (see LICENSE file for details)
 * -------------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>

#include "library.h"
#include "utils.h"

static void print_main_menu(void) {
    printf("\n");
    printf("=============================================\n");
    printf("   Simple Library Management System (C)\n");
    printf("=============================================\n");
    printf("1. List all books\n");
    printf("2. Search books by title\n");
    printf("3. Add a new book\n");
    printf("4. Borrow a book\n");
    printf("5. Return a book\n");
    printf("6. Remove a book\n");
    printf("0. Exit\n");
    printf("=============================================\n");
}

int main(void) {
    Book  *books = NULL;
    size_t book_count = 0;

    if (!library_load_books(&books, &book_count)) {
        fprintf(stderr, "Error: failed to load library data. Exiting.\n");
        return EXIT_FAILURE;
    }

    int running = 1;

    while (running) {
        print_main_menu();
        int choice = utils_read_int("Enter your choice: ", 0, 6);

        switch (choice) {
            case 1:
                /* List all books */
                library_list_books(books, book_count);
                utils_press_enter_to_continue();
                break;

            case 2:
                /* Search by title */
                library_search_books_by_title(books, book_count);
                utils_press_enter_to_continue();
                break;

            case 3:
                /* Add book */
                if (library_add_book(&books, &book_count)) {
                    if (!library_save_books(books, book_count)) {
                        fprintf(stderr, "Warning: failed to save after adding book.\n");
                    }
                }
                utils_press_enter_to_continue();
                break;

            case 4:
                /* Borrow book */
                if (library_borrow_book(books, book_count)) {
                    if (!library_save_books(books, book_count)) {
                        fprintf(stderr, "Warning: failed to save after borrowing book.\n");
                    }
                }
                utils_press_enter_to_continue();
                break;

            case 5:
                /* Return book */
                if (library_return_book(books, book_count)) {
                    if (!library_save_books(books, book_count)) {
                        fprintf(stderr, "Warning: failed to save after returning book.\n");
                    }
                }
                utils_press_enter_to_continue();
                break;

            case 6:
                /* Remove book */
                if (library_remove_book(books, &book_count)) {
                    if (!library_save_books(books, book_count)) {
                        fprintf(stderr, "Warning: failed to save after removing book.\n");
                    }
                }
                utils_press_enter_to_continue();
                break;

            case 0:
                running = 0;
                break;

            default:
                /* Should not happen due to input validation */
                printf("Unknown option.\n");
                utils_press_enter_to_continue();
                break;
        }
    }

    /* Final save in case something changed but was not saved
       (defensive, though code already saves after each mutation). */
    if (!library_save_books(books, book_count)) {
        fprintf(stderr, "Warning: failed to save library data on exit.\n");
    }

    free(books);
    books = NULL;

    printf("Goodbye.\n");
    return EXIT_SUCCESS;
}
