#ifndef PILA_H
#define PILA_H

#include <stddef.h>

#define MAX_TEXTO 4096

typedef struct NodoPila {
    char texto[MAX_TEXTO];
    struct NodoPila *siguiente;
} NodoPila;

typedef struct {
    NodoPila *tope;
    size_t tamanio;
} Pila;

void inicializarPila(Pila *p);
int pilaVacia(Pila *p);
void push(Pila *p, const char *texto);
char *pop(Pila *p);
char *peek(Pila *p);
void limpiarPila(Pila *p);

#endif
