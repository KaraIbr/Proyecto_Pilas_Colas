#ifndef IMPRESION_H
#define IMPRESION_H

#include "cola.h"

typedef struct {
    Cola colaAlta;
    Cola colaNormal;
    Cola colaBaja;
    int siguienteId;
    int hayTrabajoEnProceso;
} GestorImpresion;

void inicializarGestor(GestorImpresion *g);
void agregarTrabajo(GestorImpresion *g, const char *texto, int prioridad, int numPaginas);
void despacharTrabajo(GestorImpresion *g);
void mostrarEstadoImpresion(GestorImpresion *g);

#endif
