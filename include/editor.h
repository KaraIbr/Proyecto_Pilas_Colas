#ifndef EDITOR_H
#define EDITOR_H

#include "pila.h"

typedef struct {
    char textoActual[MAX_TEXTO];
    Pila historial;
} Editor;

void inicializarEditor(Editor *e);
void mostrarTexto(Editor *e);
void agregarTexto(Editor *e, const char *nuevoTexto);
void borrarUltimaLinea(Editor *e);
void deshacer(Editor *e);

#endif
