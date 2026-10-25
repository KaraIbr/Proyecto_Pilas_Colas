#ifndef COLA_H
#define COLA_H

#include <stddef.h>

#define MAX_TEXTO 4096

typedef struct Trabajo {
    int id;
    int prioridad; /* 1=Alta, 2=Normal, 3=Baja */
    int numPaginas;
    char texto[MAX_TEXTO];
    struct Trabajo *siguiente;
} Trabajo;

typedef struct Cola {
    Trabajo *frente;
    Trabajo *final;
    int contador;
} Cola;

void inicializarCola(Cola *c);
int colaVacia(Cola *c);
void enqueue(Cola *c, Trabajo *trabajo);
Trabajo *dequeue(Cola *c);

#endif
