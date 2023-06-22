include setup.mk

CC = gcc
PROFILE ?= DEBUG

CCFLAGS_DEBUG = -g -O0 -fno-builtin -DDEBUG
CCFLAGS_RELEASE = -O2

SRC_DIR := src
OBJ_DIR := $(PROJ_DIR)/obj
INC_DIR := $(PROJ_DIR)/include
BIN_DIR := $(PROJ_DIR)/bin
TEST_DIR := $(PROJ_DIR)/test

SRCS := $(wildcard $(SRC_DIR)/*.c)
# TODO: Fix expansion. Expands as /home/neil/.../obj/src/%.o
DEPS := $(patsubst %.c, $(INC_DIR)/%.h, $(SRCS))
OBJS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

CCFLAGS += $(CCFLAGS_$(PROFILE)) -I$(INC_DIR) -Werror -Wall -Wextra -Wformat -ansi -pedantic -std=c99
LDFLAGS += -lc -lcheck

BINS := $(BIN_DIR)/libdsc.a $(BIN_DIR)/libdsc.so

# Create static and dynamic libraries
all: $(BINS)

# Copy libraries to /usr/lib
install: all
	cp -t /usr/lib/ $(BINS)

# Remove object files and binaries
clean:
	rm -rf $(OBJ_DIR)/*.o $(BINS)

# Rebuild project
rebuild: clean all

# Create static library
$(BIN_DIR)/libdsc.a: $(OBJS)
	ar rcs $@ $(OBJS)

# Create dynamic library
$(BIN_DIR)/libdsc.so: $(OBJS)
	$(CC) -o $@ $^ -shared -fPIC $(CCFLAGS) $(LDFLAGS)
	strip $(BIN_DIR)/libdsc.so

# Create objects
$(OBJ_DIR)/%.o: $(SRCS) $(DEPS)
	$(CC) $< -c -o $@ $(CCFLAGS)

# TODO: Modify test to include all tests
test: $(BIN_DIR)/buffer_test
	./$(BIN_DIR)/buffer_test

$(BIN_DIR)/buffer_test: $(OBJS)
	$(CC) -c $(TEST_DIR)/buffer_test.c -o $(OBJ_DIR)/buffer_test.o $(CCFLAGS)
	$(CC) $(OBJ_DIR)/buffer_test.o $(OBJ_DIR)/buffer.o -o $@ $(CCFLAGS) $(LDFLAGS)

.PHONY: all install clean rebuild test
