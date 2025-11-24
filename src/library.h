/*
 * Project: Simple Library Management System
 * File: library.h
 * Author: Mobin Yousefi (GitHub: https://github.com/mobinyousefi-cs)
 * Created: 2025-11-24
 * Updated: 2025-11-24
 * License: MIT License (see LICENSE file for details)
 * -------------------------------------------------------------------------------------------------
 */

#ifndef LIBRARY_H
#define LIBRARY_H

#include <stddef.h>
#include <stdbool.h>

#define TITLE_MAX_LEN 100
#define AUTHOR_MAX_LEN 100
#define ISBN_MAX_LEN 20
#define NAME_MAX_LEN 100
#define DATE_MAX_LEN 11   /* "YYYY-MM-DD" + '\0' */

/* Core representation of a book in the library. */
typedef struct {
    int  id;
    char title[TITLE_MAX_LEN];
    char author[AUTHOR_MAX_LEN];
    char isbn[ISBN_MAX_LEN];
    int  is_borrowed;                   /* 0 = available, 1 = borrowed */
    char borrower[NAME_MAX_LEN];        /* Name of borrower, empty if not borrowed */
    char due_date[DATE_MAX_LEN];        /* Due date string, empty if not borrowed */
} Book;

/* Persistence API */
bool   library_load_books(Book **books, size_t *count);
bool   library_save_books(const Book *books, size_t count);

/* Query helpers */
int    library_find_book_index_by_id(const Book *books, size_t count, int id);

/* Operations that do not change structure size */
void   library_list_books(const Book *books, size_t count);
void   library_search_books_by_title(const Book *books, size_t count);

/* Mutating operations (may change count or book states) */
bool   library_add_book(Book **books, size_t *count);
bool   library_borrow_book(Book *books, size_t count);
bool   library_return_book(Book *books, size_t count);
bool   library_remove_book(Book *books, size_t *count);

#endif /* LIBRARY_H */
