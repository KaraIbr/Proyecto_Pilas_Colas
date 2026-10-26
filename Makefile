# =====================================================================
# Makefile - Proyecto Integrador Unidad 4: Pilas y Colas
# Compilacion automatizada con GCC y advertencias activadas (-Wall)
# =====================================================================

CC       = gcc
CFLAGS   = -Wall -Wextra -std=c99 -Iinclude
BUILDDIR = build
BINDIR   = bin

PROGRAMA = $(BINDIR)/editor_impresion
PRUEBAS  = $(BINDIR)/pruebas_impresion

# Todos los .c de src/ excepto los arneses de prueba
FUENTES  = $(filter-out src/pruebas_%.c, $(wildcard src/*.c))
OBJETOS  = $(patsubst src/%.c, $(BUILDDIR)/%.o, $(FUENTES))

# Modulos que necesita el arnes de pruebas del gestor de impresion
OBJ_PRUEBAS = $(BUILDDIR)/cola.o $(BUILDDIR)/impresion.o $(BUILDDIR)/pruebas_impresion.o
OBJ_PRUEBAS_ED = $(BUILDDIR)/pila.o $(BUILDDIR)/editor.o $(BUILDDIR)/pruebas_editor.o
PRUEBAS_ED = $(BINDIR)/pruebas_editor

.PHONY: all pruebas correr-pruebas pruebas-editor limpiar

all: $(PROGRAMA)

$(PROGRAMA): $(OBJETOS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $(OBJETOS)

# Arnes de pruebas unitarias del gestor de impresion (no requiere main.c)
pruebas: $(PRUEBAS)

$(PRUEBAS): $(OBJ_PRUEBAS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $(OBJ_PRUEBAS)

correr-pruebas: $(PRUEBAS)
	./$(PRUEBAS)

$(BUILDDIR)/%.o: src/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

limpiar:
	rm -rf $(BUILDDIR) $(BINDIR)

$(PRUEBAS_ED): $(OBJ_PRUEBAS_ED) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $(OBJ_PRUEBAS_ED)

pruebas-editor: $(PRUEBAS_ED)
	./$(PRUEBAS_ED)
