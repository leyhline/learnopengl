SHELL = /bin/sh

objects = app.o glad.o
srcdir = src

OBJS = $(patsubst %,$(srcdir)/%,$(objects))
CFLAGS = -std=c11 -Iinclude -Wall -O0 -g
LDFLAGS = -Llib
LDLIBS = -lglfw3 -lm -lrt -ldl -lpthread -lwayland-client

.SUFFIXES: .c .o

app: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

.PHONY: clean
clean:
	-rm app $(OBJS)