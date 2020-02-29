SHELL = /bin/sh

OBJS = app.o glad.o shader.o resources.o
CFLAGS = -std=c11 -Iinclude -Wall -O0 -g
LDFLAGS = -Llib
LDLIBS = -lglfw3 -lm -lrt -ldl -lpthread -lwayland-client

.SUFFIXES: .c .o

app: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

app.o: glad.c

.PHONY: clean
clean:
	-rm app $(OBJS)