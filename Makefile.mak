# Project: Simple Library Management System
# Author: Mobin Yousefi (GitHub: https://github.com/mobinyousefi-cs)

CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -pedantic -O2
LDFLAGS :=

SRC_DIR := src
SRCS    := $(SRC_DIR)/main.c \
           $(SRC_DIR)/library.c \
           $(SRC_DIR)/utils.c

OBJS    := $(SRCS:.c=.o)
TARGET  := library_management

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
