#include "editor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inicializarEditor(Editor *e) {
    if (e == NULL) {
        return;
    }
    e->textoActual[0] = '\0';
    inicializarPila(&e->historial);
}

void mostrarTexto(Editor *e) {
    size_t longitud;

    if (e == NULL) {
        return;
    }

    printf("Texto actual:\n");
    longitud = strlen(e->textoActual);
    if (longitud == 0) {
        printf("(vacio)\n");
        return;
    }

    printf("%s", e->textoActual);
    if (e->textoActual[longitud - 1] != '\n') {
        printf("\n");
    }
}

void agregarTexto(Editor *e, const char *nuevoTexto) {
    size_t actual;
    size_t nuevo;
    size_t disponible;
    size_t copiar;
    size_t tamanioAntes;

    if (e == NULL || nuevoTexto == NULL || nuevoTexto[0] == '\0') {
        return;
    }

    actual = strlen(e->textoActual);
    if (actual >= MAX_TEXTO - 1) {
        return;
    }

    tamanioAntes = e->historial.tamanio;
    push(&e->historial, e->textoActual);
    if (e->historial.tamanio == tamanioAntes) {
        return;
    }

    nuevo = strlen(nuevoTexto);
    disponible = (MAX_TEXTO - 1) - actual;
    copiar = (nuevo < disponible) ? nuevo : disponible;

    memmove(e->textoActual + actual, nuevoTexto, copiar);
    actual += copiar;

    if (actual < MAX_TEXTO - 1 && (actual == 0 || e->textoActual[actual - 1] != '\n')) {
        e->textoActual[actual] = '\n';
        actual++;
    }
    e->textoActual[actual] = '\0';
}

void borrarUltimaLinea(Editor *e) {
    size_t longitud;
    size_t fin;
    size_t corte;
    size_t tamanioAntes;

    if (e == NULL) {
        return;
    }

    longitud = strlen(e->textoActual);
    if (longitud == 0) {
        printf("No hay texto para borrar.\n");
        return;
    }

    tamanioAntes = e->historial.tamanio;
    push(&e->historial, e->textoActual);
    if (e->historial.tamanio == tamanioAntes) {
        return;
    }

    fin = longitud;
    if (fin > 0 && e->textoActual[fin - 1] == '\n') {
        fin--;
    }

    corte = fin;
    while (corte > 0 && e->textoActual[corte - 1] != '\n') {
        corte--;
    }

    e->textoActual[corte] = '\0';
}

void deshacer(Editor *e) {
    char *anterior;
    size_t longitud;

    if (e == NULL) {
        return;
    }

    if (pilaVacia(&e->historial)) {
        printf("No hay cambios para deshacer.\n");
        return;
    }

    anterior = pop(&e->historial);
    if (anterior == NULL) {
        printf("No se pudo recuperar el estado anterior.\n");
        return;
    }

    longitud = strlen(anterior);
    if (longitud >= MAX_TEXTO) {
        longitud = MAX_TEXTO - 1;
    }
    memcpy(e->textoActual, anterior, longitud);
    e->textoActual[longitud] = '\0';
    free(anterior);

    printf("Deshacer aplicado.\n");
}
