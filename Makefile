CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
SOURCES = 
OBJECTS = 
TARGET = /proyecto_pilas_colas

all: 

:
	mkdir -p 

:
	mkdir -p 

:  | 
	  -o 

/%.o: /%.c | 
	  -c $< -o $@

clean:
	rm -rf  

run: 
	./

rebuild: clean all

.PHONY: all clean run rebuild
