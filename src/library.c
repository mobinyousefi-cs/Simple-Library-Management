/*
 * Project: Simple Library Management System
 * File: library.c
 * Author: Mobin Yousefi (GitHub: https://github.com/mobinyousefi-cs)
 * Created: 2025-11-24
 * Updated: 2025-11-24
 * License: MIT License (see LICENSE file for details)
 * -------------------------------------------------------------------------------------------------
 */

#include "library.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *BOOKS_DATA_FILE = "books.dat";

/* Internal helpers */
static void library_print_book(const Book *book);
static int  library_get_next_id(const Book *books, size_t count);

/* -------------------------------------------------------------------------------------------------
 * Persistence
 * -------------------------------------------------------------------------------------------------
 */

bool library_load_books(Book **books, size_t *count) {
    if (books == NULL || count == NULL) {
        return false;
    }

    *books = NULL;
    *count = 0;

    FILE *fp = fopen(BOOKS_DATA_FILE, "rb");
    if (!fp) {
        /* File does not exist yet: treat as empty library. */
        return true;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return false;
    }

    long file_size = ftell(fp);
    if (file_size < 0) {
        fclose(fp);
        return false;
    }

    if (file_size == 0) {
        /* Empty file: no books. */
        fclose(fp);
        return true;
    }

    if (file_size % (long)sizeof(Book) != 0) {
        /* Corrupted file (size not multiple of struct size). */
        fclose(fp);
        return false;
    }

    size_t num_books = (size_t)(file_size / (long)sizeof(Book));

    if (fseek(fp, 0, SEEK_SET) != 0) {
        fclose(fp);
        return false;
    }

    Book *buffer = (Book *)malloc(num_books * sizeof(Book));
    if (!buffer) {
        fclose(fp);
        return false;
    }

    size_t read_count = fread(buffer, sizeof(Book), num_books, fp);
    fclose(fp);

    if (read_count != num_books) {
        free(buffer);
        return false;
    }

    *books = buffer;
    *count = num_books;
    return true;
}

bool library_save_books(const Book *books, size_t count) {
    FILE *fp = fopen(BOOKS_DATA_FILE, "wb");
    if (!fp) {
        perror("Failed to open data file for writing");
        return false;
    }

    if (count > 0 && books != NULL) {
        size_t written = fwrite(books, sizeof(Book), count, fp);
        if (written != count) {
            perror("Failed to write data file");
            fclose(fp);
            return false;
        }
    }

    fclose(fp);
    return true;
}

/* -------------------------------------------------------------------------------------------------
 * Core helpers
 * -------------------------------------------------------------------------------------------------
 */

int library_find_book_index_by_id(const Book *books, size_t count, int id) {
    if (!books) {
        return -1;
    }

    for (size_t i = 0; i < count; ++i) {
        if (books[i].id == id) {
            return (int)i;
        }
    }

    return -1;
}

static void library_print_book(const Book *book) {
    if (!book) {
        return;
    }

    printf("------------------------------------------------------------\n");
    printf("ID        : %d\n", book->id);
    printf("Title     : %s\n", book->title);
    printf("Author    : %s\n", book->author);
    printf("ISBN      : %s\n", book->isbn);

    if (book->is_borrowed) {
        printf("Status    : Borrowed\n");
        printf("Borrower  : %s\n", book->borrower);
        printf("Due Date  : %s\n", book->due_date);
    } else {
        printf("Status    : Available\n");
    }
}

static int library_get_next_id(const Book *books, size_t count) {
    int max_id = 0;
    for (size_t i = 0; i < count; ++i) {
        if (books[i].id > max_id) {
            max_id = books[i].id;
        }
    }
    return max_id + 1;
}

/* -------------------------------------------------------------------------------------------------
 * Public operations (non-mutating)
 * -------------------------------------------------------------------------------------------------
 */

void library_list_books(const Book *books, size_t count) {
    if (count == 0 || books == NULL) {
        printf("No books in the library.\n");
        return;
    }

    printf("\n=== Library Inventory (%zu book%s) ===\n",
           count, (count == 1 ? "" : "s"));

    for (size_t i = 0; i < count; ++i) {
        library_print_book(&books[i]);
    }
}

void library_search_books_by_title(const Book *books, size_t count) {
    if (count == 0 || books == NULL) {
        printf("No books in the library.\n");
        return;
    }

    char query[TITLE_MAX_LEN];
    utils_read_line("Enter title keyword to search: ", query, sizeof(query));

    if (query[0] == '\0') {
        printf("Search term is empty. Aborting search.\n");
        return;
    }

    int matches = 0;
    printf("\nSearch results for \"%s\":\n", query);

    for (size_t i = 0; i < count; ++i) {
        if (strstr(books[i].title, query) != NULL) {
            library_print_book(&books[i]);
            ++matches;
        }
    }

    if (matches == 0) {
        printf("No books found matching the given keyword.\n");
    } else {
        printf("Total matches: %d\n", matches);
    }
}

/* -------------------------------------------------------------------------------------------------
 * Public operations (mutating)
 * -------------------------------------------------------------------------------------------------
 */

bool library_add_book(Book **books, size_t *count) {
    if (books == NULL || count == NULL) {
        return false;
    }

    Book *current = *books;
    size_t current_count = *count;

    Book new_book;
    memset(&new_book, 0, sizeof(new_book));

    utils_read_line("Enter book title: ", new_book.title, sizeof(new_book.title));
    utils_read_line("Enter book author: ", new_book.author, sizeof(new_book.author));
    utils_read_line("Enter book ISBN: ", new_book.isbn, sizeof(new_book.isbn));

    new_book.id = library_get_next_id(current, current_count);
    new_book.is_borrowed = 0;
    new_book.borrower[0] = '\0';
    new_book.due_date[0] = '\0';

    Book *resized = (Book *)realloc(current, (current_count + 1) * sizeof(Book));
    if (!resized) {
        printf("Error: failed to allocate memory for new book.\n");
        return false;
    }

    resized[current_count] = new_book;

    *books = resized;
    *count = current_count + 1;

    printf("Book added successfully with ID %d.\n", new_book.id);
    return true;
}

bool library_borrow_book(Book *books, size_t count) {
    if (books == NULL || count == 0) {
        printf("No books available to borrow.\n");
        return false;
    }

    int id = utils_read_int("Enter the ID of the book to borrow: ", 1, 1000000000);

    int index = library_find_book_index_by_id(books, count, id);
    if (index < 0) {
        printf("Book with ID %d not found.\n", id);
        return false;
    }

    Book *book = &books[index];
    if (book->is_borrowed) {
        printf("Book \"%s\" is already borrowed.\n", book->title);
        return false;
    }

    char borrower[NAME_MAX_LEN];
    char due_date[DATE_MAX_LEN];

    utils_read_line("Enter borrower name: ", borrower, sizeof(borrower));
    utils_read_line("Enter due date (YYYY-MM-DD): ", due_date, sizeof(due_date));

    strncpy(book->borrower, borrower, sizeof(book->borrower) - 1);
    book->borrower[sizeof(book->borrower) - 1] = '\0';

    strncpy(book->due_date, due_date, sizeof(book->due_date) - 1);
    book->due_date[sizeof(book->due_date) - 1] = '\0';

    book->is_borrowed = 1;

    printf("Book \"%s\" successfully borrowed by %s.\n", book->title, book->borrower);
    return true;
}

bool library_return_book(Book *books, size_t count) {
    if (books == NULL || count == 0) {
        printf("No books available.\n");
        return false;
    }

    int id = utils_read_int("Enter the ID of the book to return: ", 1, 1000000000);

    int index = library_find_book_index_by_id(books, count, id);
    if (index < 0) {
        printf("Book with ID %d not found.\n", id);
        return false;
    }

    Book *book = &books[index];
    if (!book->is_borrowed) {
        printf("Book \"%s\" is not currently borrowed.\n", book->title);
        return false;
    }

    book->is_borrowed = 0;
    book->borrower[0] = '\0';
    book->due_date[0] = '\0';

    printf("Book \"%s\" has been returned and is now available.\n", book->title);
    return true;
}

bool library_remove_book(Book *books, size_t *count) {
    if (books == NULL || count == NULL || *count == 0) {
        printf("No books to remove.\n");
        return false;
    }

    int id = utils_read_int("Enter the ID of the book to remove: ", 1, 1000000000);
    int index = library_find_book_index_by_id(books, *count, id);

    if (index < 0) {
        printf("Book with ID %d not found.\n", id);
        return false;
    }

    /* Shift following elements left by one (no realloc, keeps it simple). */
    for (size_t i = (size_t)index; i + 1 < *count; ++i) {
        books[i] = books[i + 1];
    }

    (*count)--;

    printf("Book with ID %d removed from library.\n", id);
    return true;
}
