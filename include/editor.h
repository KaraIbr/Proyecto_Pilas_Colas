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
/* Escribe el documento actual en el archivo indicado.
 * Devuelve 0 si se guardo, 1 si los argumentos no son validos,
 * 2 si no se pudo abrir el archivo y 3 si fallo la escritura. */
int guardarDocumento(Editor *e, const char *ruta);
void deshacer(Editor *e);

#endif
