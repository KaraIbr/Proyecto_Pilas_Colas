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
/* Elimina del final la cantidad de caracteres indicada. */
void borrarCaracteres(Editor *e, int cantidad);
/* Muestra el contenido de la pila de historial sin modificarla. */
void mostrarHistorial(Editor *e);
void deshacer(Editor *e);

#endif
