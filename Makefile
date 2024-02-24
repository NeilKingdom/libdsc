CC = gcc
PROFILE ?= DEBUG

CCFLAGS_DEBUG = -ggdb -O0 -fno-builtin -DDEBUG
CCFLAGS_RELEASE = -O2

SRC_DIR := src
OBJ_DIR := obj
INC_DIR := include
BIN_DIR := bin
TEST_DIR := test

TGT_INC_DIR := /usr/include/
TGT_BIN_DIR := /usr/lib/

SRCS := $(wildcard $(SRC_DIR)/*.c)
DEPS := $(wildcard $(SRC_DIR)/*.h)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

CCFLAGS += $(CCFLAGS_$(PROFILE)) -I$(INC_DIR) -std=c99 -Wall -Wextra -Wformat -Werror
LDFLAGS += -lc -lcheck

BINS := $(BIN_DIR)/libdsc.a $(BIN_DIR)/libdsc.so

# Create static and dynamic libraries
all: $(BINS)

# Copy libraries to /usr/lib
install: all
	cp $(BINS) $(TGT_BIN_DIR)
	cp $(INC_DIR)/*.h $(TGT_INC_DIR)

# Remove object files and binaries
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*

# Rebuild project
rebuild: clean all

# Create static library
$(BIN_DIR)/libdsc.a: $(OBJS)
	ar rcs $@ $^

# Create dynamic library
$(BIN_DIR)/libdsc.so: $(OBJS)
	$(CC) -o $@ $^ -shared -fPIC $(CCFLAGS) $(LDFLAGS)
	strip ./bin/libdsc.so

# Create objects
$(OBJ_DIR)/%.o: $(SRCS) $(DEPS)
	$(CC) $< -c -o $@ $(CCFLAGS)

# TODO: Modify test to include all tests
test: all

.PHONY: all install clean rebuild test
