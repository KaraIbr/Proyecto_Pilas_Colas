#include "ui.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

#define ANCHO_CAJA 60

static void configurarConsola(void) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

static void imprimirBorde(const char *izquierda, const char *derecha) {
    int i;

    printf("%s", izquierda);
    for (i = 0; i < ANCHO_CAJA; i++) {
        printf("═");
    }
    printf("%s\n", derecha);
}

static void imprimirFilaMenu(const char *texto) {
    printf("║ %-*s ║\n", ANCHO_CAJA - 2, texto);
}

void limpiarPantalla(void) {
    configurarConsola();
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void mostrarMenu(void) {
    configurarConsola();

    imprimirBorde("╔", "╗");
    imprimirFilaMenu("        EDITOR DE TEXTO Y GESTOR DE IMPRESION");
    imprimirBorde("╠", "╣");
    imprimirFilaMenu("  1. Agregar texto");
    imprimirFilaMenu("  2. Borrar ultima linea");
    imprimirFilaMenu("  3. Deshacer");
    imprimirFilaMenu("  4. Mostrar texto actual");
    imprimirFilaMenu("  5. Enviar texto a impresion");
    imprimirFilaMenu("  6. Despachar siguiente trabajo");
    imprimirFilaMenu("  7. Ver estado de impresion");
    imprimirFilaMenu("  8. Salir");
    imprimirBorde("╠", "╣");
    imprimirFilaMenu("  [TOPE] cima de la pila de historial");
    imprimirFilaMenu("  [ALTA] prioridad alta de impresion");
    imprimirBorde("╚", "╝");

    printf("\nSeleccione una opcion: ");
    fflush(stdout);
}

void mostrarLineaSeparadora(void) {
    int i;

    configurarConsola();
    for (i = 0; i < ANCHO_CAJA; i++) {
        printf("═");
    }
    printf("\n");
}

void esperarEnter(void) {
    int c;

    configurarConsola();
    printf("\nPresione ENTER para continuar...");
    fflush(stdout);

#ifdef _WIN32
    do {
        c = _getch();
    } while (c != '\r' && c != '\n' && c != EOF);
#else
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
#endif
    printf("\n");
}
