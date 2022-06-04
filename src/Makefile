include setup.mk

SRC = $(wildcard *.c)

DDIR = $(PROJ_DIR)/include
DEPS = $(patsubst %.c, $(DDIR)/%.h, $(SRC))

ODIR = $(PROJ_DIR)/obj
OBJ = $(patsubst %.c, $(ODIR)/%.o, $(SRC))

CC = gcc
RELEASE = debug

# TODO: Add logic to switch depending on RELEASE
DEBUG = -g -DDEBUG -Werror
RELEASE =
CFLAGS += $(DEBUG) -I$(DDIR) -Wall -Wextra -O2
LDFLAGS += -lc
BIN := libdsc.a libdsc.so

.PHONY: all
all: libdsc.a libdsc.so

libdsc.a:

libdsc.so:

.PHONY: clean
clean:
	rm -f $(BIN) $(ODIR)/*.o
