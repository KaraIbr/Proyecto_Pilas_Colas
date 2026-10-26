/* =====================================================================
 * cola.c - Implementacion de las primitivas FIFO de la cola
 *
 * Proyecto Integrador Unidad 4: Pilas y Colas
 * Responsable: Aaron Salvador Castaneda Ruiz
 *
 * Invariantes que mantiene este modulo en todo momento:
 *   - Si cantidad == 0  =>  frente == NULL y final == NULL
 *   - Si cantidad == 1  =>  frente == final
 *   - Si cantidad >  1  =>  frente != final y final->siguiente == NULL
 * ===================================================================== */

#include <stdlib.h>
#include <string.h>

#include "cola.h"

void cola_inicializar(Cola *c)
{
    if (c == NULL) {
        return;
    }
    c->frente   = NULL;
    c->final    = NULL;
    c->cantidad = 0;
}

bool cola_esta_vacia(const Cola *c)
{
    /* Se valida por el puntero FRENTE, que es la definicion estructural
     * de cola vacia; el contador sirve solo como dato de apoyo. */
    return (c == NULL) || (c->frente == NULL);
}

int cola_cantidad(const Cola *c)
{
    return (c == NULL) ? 0 : c->cantidad;
}

bool cola_encolar(Cola *c, const Trabajo *t)
{
    NodoCola *nuevo;

    if (c == NULL || t == NULL) {
        return false;
    }

    nuevo = (NodoCola *) malloc(sizeof(NodoCola));
    if (nuevo == NULL) {
        return false;   /* sin memoria disponible */
    }

    /* Copia por valor del trabajo completo, incluido el texto del
     * documento: a partir de aqui el trabajo encolado es inmutable
     * frente a cambios posteriores en el editor. */
    nuevo->trabajo   = *t;
    nuevo->siguiente = NULL;

    if (c->final == NULL) {
        /* Cola vacia: el nuevo nodo es a la vez frente y final. */
        c->frente = nuevo;
        c->final  = nuevo;
    } else {
        /* Caso general: se engancha al FINAL, nunca al frente. */
        c->final->siguiente = nuevo;
        c->final            = nuevo;
    }

    c->cantidad++;
    return true;
}

bool cola_desencolar(Cola *c, Trabajo *salida)
{
    NodoCola *retirado;

    if (c == NULL || salida == NULL || cola_esta_vacia(c)) {
        return false;   /* validacion de cola vacia */
    }

    /* Siempre sale el del FRENTE: disciplina FIFO estricta. */
    retirado = c->frente;
    *salida  = retirado->trabajo;

    c->frente = retirado->siguiente;
    if (c->frente == NULL) {
        /* Se retiro el ultimo elemento: tambien hay que soltar FINAL. */
        c->final = NULL;
    }

    free(retirado);
    c->cantidad--;
    return true;
}

bool cola_consultar_frente(const Cola *c, Trabajo *salida)
{
    if (c == NULL || salida == NULL || cola_esta_vacia(c)) {
        return false;
    }

    /* Consultar NO altera la cola: no se desencola ni se reordena. */
    *salida = c->frente->trabajo;
    return true;
}

void cola_vaciar(Cola *c)
{
    NodoCola *actual;
    NodoCola *siguiente;

    if (c == NULL) {
        return;
    }

    actual = c->frente;
    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }

    cola_inicializar(c);
}
