include setup.mk

CC = gcc
PROFILE ?= DEBUG

CCFLAGS_DEBUG = -g -O0 -fno-builtin -DDEBUG
CCFLAGS_RELEASE = -O2

OBJ_DIR := $(PROJ_DIR)/obj
INC_DIR := $(PROJ_DIR)/include
BIN_DIR := $(PROJ_DIR)/bin

SRCS = $(wildcard *.c)
DEPS = $(patsubst %.c, $(INC_DIR)/%.h, $(SRCS))
OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

CCFLAGS += $(CCFLAGS_$(PROFILE)) -I$(INC_DIR) -Werror -Wall -Wextra -ansi -pedantic -std=c99
LDFLAGS += -lc

BINS := $(BIN_DIR)/libdsc.a $(BIN_DIR)/libdsc.so

# Create static and dynamic libraries
.PHONY: all
all: $(BINS)

# Copy libraries to /usr/lib
.PHONY: install
install: all
	cp -t /usr/lib/ $(BINS)

# Remove object files and binaries
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(BINS)

# Rebuild project
.PHONY: rebuild
rebuild: clean all

# Create static library
$(BIN_DIR)/libdsc.a: $(OBJS)
	ar rcs $@ $(OBJS)

# Create dynamic library
$(BIN_DIR)/libdsc.so: $(OBJS)
	$(CC) -o $@ $^ -shared -fPIC $(CCFLAGS) $(LDFLAGS)

# Create objects
$(OBJ_DIR)/%.o: %.c $(DEPS)
	$(CC) $< -c -o $@ $(CCFLAGS)
