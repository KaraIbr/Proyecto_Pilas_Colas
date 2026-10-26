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
    imprimirFilaMenu("  EDITOR");
    imprimirFilaMenu("   1. Crear documento nuevo");
    imprimirFilaMenu("   2. Agregar texto al final");
    imprimirFilaMenu("   3. Eliminar caracteres del final");
    imprimirFilaMenu("   4. Borrar ultima linea");
    imprimirFilaMenu("   5. Mostrar texto actual");
    imprimirFilaMenu("   6. Mostrar pila de historial");
    imprimirFilaMenu("   7. Deshacer");
    imprimirBorde("╠", "╣");
    imprimirFilaMenu("  IMPRESION");
    imprimirFilaMenu("   8. Enviar documento a impresion");
    imprimirFilaMenu("   9. Iniciar siguiente trabajo");
    imprimirFilaMenu("  10. Finalizar trabajo en curso");
    imprimirFilaMenu("  11. Ver estado de las colas");
    imprimirFilaMenu("  12. Ver historial de impresion");
    imprimirBorde("╠", "╣");
    imprimirFilaMenu("   0. Salir");
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
