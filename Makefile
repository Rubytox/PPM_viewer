CC=gcc
LD=gcc
CFLAGS= -std=c99 -Wall -Wextra -g
LDFLAGS=-lSDL

BIN_DIR=bin
OBJ_DIR=obj
SRC_DIR=src
OBJ_FILES = $(OBJ_DIR)/ppm_viewer.o

TARGET=$(BIN_DIR)/ppm_viewer

all: $(OBJ_DIR)/ppm_viewer.o
	$(LD) $(LDFLAGS) $(OBJ_FILES) -o $(TARGET)

$(OBJ_DIR)/ppm_viewer.o: $(SRC_DIR)/ppm_viewer.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/ppm_viewer.c -o $(OBJ_DIR)/ppm_viewer.o

.PHONY: clean

clean:
	rm -rf bin/*
	rm -rf obj/*
	rm -rf $(TARGET)
