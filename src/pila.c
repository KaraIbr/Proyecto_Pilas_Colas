#include "pila.h"

#include <stdlib.h>
#include <string.h>

void inicializarPila(Pila *p) {
    if (p == NULL) {
        return;
    }
    p->tope = NULL;
    p->tamanio = 0;
}

int pilaVacia(Pila *p) {
    return p == NULL || p->tope == NULL;
}

void push(Pila *p, const char *texto) {
    NodoPila *nuevo;
    const char *origen;
    size_t longitud;

    if (p == NULL) {
        return;
    }

    nuevo = (NodoPila *)malloc(sizeof(NodoPila));
    if (nuevo == NULL) {
        return;
    }

    origen = (texto != NULL) ? texto : "";
    longitud = strlen(origen);
    if (longitud >= MAX_TEXTO) {
        longitud = MAX_TEXTO - 1;
    }
    memcpy(nuevo->texto, origen, longitud);
    nuevo->texto[longitud] = '\0';

    nuevo->siguiente = p->tope;
    p->tope = nuevo;
    p->tamanio++;
}

char *pop(Pila *p) {
    NodoPila *nodo;
    char *copia;
    size_t longitud;

    if (pilaVacia(p)) {
        return NULL;
    }

    nodo = p->tope;
    longitud = strlen(nodo->texto);
    copia = (char *)malloc(longitud + 1);
    if (copia == NULL) {
        return NULL;
    }
    memcpy(copia, nodo->texto, longitud + 1);

    p->tope = nodo->siguiente;
    p->tamanio--;
    free(nodo);

    return copia;
}

char *peek(Pila *p) {
    if (pilaVacia(p)) {
        return NULL;
    }
    return p->tope->texto;
}

void limpiarPila(Pila *p) {
    NodoPila *actual;

    if (p == NULL) {
        return;
    }

    actual = p->tope;
    while (actual != NULL) {
        NodoPila *siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }

    p->tope = NULL;
    p->tamanio = 0;
}
