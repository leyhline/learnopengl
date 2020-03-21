SHELL = /bin/sh

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))
CFLAGS = -std=c11 -Wall -O0 -g
LDFLAGS = -Llib
LDLIBS = -lglfw3 -lm -lrt -ldl -lpthread -lwayland-client

.SUFFIXES: .c .o

app: $(OBJ)

.PHONY: clean
clean:
	-rm app $(OBJ)