/* Pruebas del Editor: casos 5, 6 y 7 y la secuencia ilustrativa. */

#include <stdio.h>
#include <string.h>

#include "editor.h"

static int totales = 0;
static int ok = 0;

static void verificar(const char *descripcion, int condicion)
{
    totales++;
    if (condicion) {
        ok++;
        printf("    [PASA] %s\n", descripcion);
    } else {
        printf("    [FALLA] %s\n", descripcion);
    }
}

static void titulo(const char *texto)
{
    printf("\n--------------------------------------------------------\n");
    printf(" %s\n", texto);
    printf("--------------------------------------------------------\n");
}

/* Secuencia ilustrativa del enunciado */
static void secuencia_ilustrativa(void)
{
    Editor e;

    titulo("SECUENCIA: Red / Red segura / borrar 7 / deshacer x2");
    inicializarEditor(&e);
    verificar("documento nuevo esta vacio", strcmp(e.textoActual, "") == 0);
    verificar("historial nuevo esta vacio", pilaVacia(&e.historial));

    agregarTexto(&e, "Red");
    printf("    Agregar \"Red\"       -> \"%s\"\n", e.textoActual);
    verificar("texto es Red", strcmp(e.textoActual, "Red") == 0);

    agregarTexto(&e, " segura");
    printf("    Agregar \" segura\"   -> \"%s\"\n", e.textoActual);
    verificar("texto es Red segura", strcmp(e.textoActual, "Red segura") == 0);

    borrarCaracteres(&e, 7);
    printf("    Borrar 7 caracteres -> \"%s\"\n", e.textoActual);
    verificar("texto vuelve a Red", strcmp(e.textoActual, "Red") == 0);

    printf("    Estado de la pila tras 3 modificaciones:\n");
    mostrarHistorial(&e);
    verificar("la pila guarda 3 estados", e.historial.tamanio == 3);

    deshacer(&e);
    printf("    Deshacer -> \"%s\"\n", e.textoActual);
    verificar("se recupera Red segura", strcmp(e.textoActual, "Red segura") == 0);

    deshacer(&e);
    printf("    Deshacer -> \"%s\"\n", e.textoActual);
    verificar("se recupera Red", strcmp(e.textoActual, "Red") == 0);

    limpiarPila(&e.historial);
}

/* Caso 5: varias modificaciones conservan los estados anteriores */
static void caso_5(void)
{
    Editor e;

    titulo("CASO 5: varias modificaciones consecutivas");
    inicializarEditor(&e);

    agregarTexto(&e, "uno");
    agregarTexto(&e, " dos");
    agregarTexto(&e, " tres");
    borrarCaracteres(&e, 5);

    verificar("el historial guarda los 4 estados previos", e.historial.tamanio == 4);
    verificar("el TOPE es el estado inmediatamente anterior",
              strcmp(peek(&e.historial), "uno dos tres") == 0);

    limpiarPila(&e.historial);
}

/* Caso 6: Deshacer varias veces recupera en orden LIFO */
static void caso_6(void)
{
    Editor e;

    titulo("CASO 6: Deshacer varias veces seguidas (LIFO)");
    inicializarEditor(&e);

    agregarTexto(&e, "A");
    agregarTexto(&e, "B");
    agregarTexto(&e, "C");
    printf("    Texto: \"%s\"\n", e.textoActual);

    deshacer(&e);
    verificar("primer deshacer deja AB", strcmp(e.textoActual, "AB") == 0);
    deshacer(&e);
    verificar("segundo deshacer deja A", strcmp(e.textoActual, "A") == 0);
    deshacer(&e);
    verificar("tercer deshacer deja el documento vacio",
              strcmp(e.textoActual, "") == 0);
    verificar("la pila queda vacia", pilaVacia(&e.historial));

    limpiarPila(&e.historial);
}

/* Caso 7: Deshacer con pila vacia no falla ni cambia el documento */
static void caso_7(void)
{
    Editor e;

    titulo("CASO 7: Deshacer con la pila de historial vacia");
    inicializarEditor(&e);
    agregarTexto(&e, "intacto");
    deshacer(&e);                 /* vacia la pila */

    verificar("la pila esta vacia", pilaVacia(&e.historial));
    deshacer(&e);                 /* deshacer de mas */
    verificar("el programa sigue vivo tras deshacer de mas", 1);
    verificar("el documento no cambio", strcmp(e.textoActual, "") == 0);

    /* Consultar no debe apilar estados nuevos. */
    mostrarTexto(&e);
    mostrarHistorial(&e);
    verificar("consultar no agrega estados a la pila", pilaVacia(&e.historial));

    limpiarPila(&e.historial);
}

int main(void)
{
    printf("========================================================\n");
    printf(" PRUEBAS DEL EDITOR - Pila de historial y Deshacer\n");
    printf("========================================================\n");

    secuencia_ilustrativa();
    caso_5();
    caso_6();
    caso_7();

    printf("\n========================================================\n");
    printf(" RESULTADO: %d de %d verificaciones pasaron\n", ok, totales);
    printf("========================================================\n");

    return (ok == totales) ? 0 : 1;
}
