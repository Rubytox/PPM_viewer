CC=gcc
LD=gcc
CFLAGS= -std=c99 -Wall -Wextra -g
LDFLAGS=-lSDL
EXEC=ppm_viewer

all: $(EXEC)

.PHONY: clean

clean:
	rm -rf *.o
	rm -rf $(EXEC)
