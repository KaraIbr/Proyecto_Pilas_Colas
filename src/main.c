/* =====================================================================
 * main.c - Aplicacion principal
 *
 * Proyecto Integrador Unidad 4: Pilas y Colas
 * Estructuras de Datos y Compiladores
 * Integrantes: Karina Patricia Ibarra Morales
 *              Aaron Salvador Castaneda Ruiz
 *
 * Coordina los dos modulos de la aplicacion y presenta el menu:
 *
 *   - Editor de documentos, cuyo historial de cambios vive en una pila
 *     (LIFO) y permite Deshacer.
 *   - Gestor de impresion, con tres colas FIFO por nivel de prioridad.
 *
 * El punto de union es el envio a impresion: se entrega el contenido
 * del documento al gestor, que guarda una copia propia dentro del
 * trabajo. Por eso editar el documento despues de enviarlo no altera
 * lo que ya esta en la cola.
 * ===================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cola.h"
#include "impresion.h"
#include "editor.h"
#include "ui.h"

#define MAX_LINEA 256

/* --- Lectura de entrada --------------------------------------------- */

/* Lee una linea y le quita el salto de linea final.
 * Devuelve false si se cerro la entrada. */
static bool leer_linea(const char *mensaje, char *destino, int tamano)
{
    size_t largo;

    if (mensaje != NULL) {
        printf("%s", mensaje);
        fflush(stdout);
    }
    if (fgets(destino, tamano, stdin) == NULL) {
        return false;
    }
    largo = strlen(destino);
    while (largo > 0 && (destino[largo - 1] == '\n' || destino[largo - 1] == '\r')) {
        destino[largo - 1] = '\0';
        largo--;
    }
    return true;
}

/* Lee un entero y avisa si lo capturado no es un numero. */
static bool leer_entero(const char *mensaje, int *valor)
{
    char linea[MAX_LINEA];
    char sobrante;

    if (!leer_linea(mensaje, linea, sizeof(linea))) {
        return false;
    }
    if (sscanf(linea, "%d %c", valor, &sobrante) != 1) {
        printf("  [!] Entrada invalida: se esperaba un numero entero.\n");
        return false;
    }
    return true;
}

/* --- Pantallas del gestor de impresion ------------------------------ */

/* Lista los trabajos de un nivel del FRENTE al FINAL, sin modificar la cola. */
static void mostrar_cola(const GestorImpresion *g, int prioridad)
{
    const Cola     *c;
    const NodoCola *nodo;
    int             posicion;

    c = gestor_cola_de_prioridad(g, prioridad);
    if (c == NULL) {
        return;
    }

    printf("  [%-6s] %d pendiente(s)", gestor_nombre_prioridad(prioridad),
           cola_cantidad(c));
    if (cola_esta_vacia(c)) {
        printf(" -- vacia\n");
        return;
    }
    printf("\n");

    posicion = 1;
    for (nodo = c->frente; nodo != NULL; nodo = nodo->siguiente) {
        printf("      %d. #%d  %-20s %3d pag.  llegada %d  %-10s%s\n",
               posicion,
               nodo->trabajo.id,
               nodo->trabajo.nombre,
               nodo->trabajo.paginas,
               nodo->trabajo.orden_llegada,
               gestor_nombre_estado(nodo->trabajo.estado),
               (posicion == 1) ? " <- FRENTE" : "");
        posicion++;
    }
}

static void mostrar_estado(const GestorImpresion *g)
{
    Trabajo actual;
    Trabajo siguiente;

    printf("\n--- ESTADO DEL GESTOR DE IMPRESION ---\n");
    if (gestor_trabajo_en_curso(g, &actual)) {
        printf("  EN CURSO: #%d %s (%d pag., prioridad %s) [%s]\n",
               actual.id, actual.nombre, actual.paginas,
               gestor_nombre_prioridad(actual.prioridad),
               gestor_nombre_estado(actual.estado));
    } else {
        printf("  EN CURSO: impresora libre\n");
    }
    mostrar_cola(g, PRIORIDAD_ALTA);
    mostrar_cola(g, PRIORIDAD_NORMAL);
    mostrar_cola(g, PRIORIDAD_BAJA);

    if (gestor_siguiente_por_atender(g, &siguiente)) {
        printf("  SIGUIENTE POR ATENDER: #%d %s (prioridad %s)\n",
               siguiente.id, siguiente.nombre,
               gestor_nombre_prioridad(siguiente.prioridad));
    } else {
        printf("  SIGUIENTE POR ATENDER: ninguno\n");
    }
    printf("  Pendientes: %d    Completados: %d\n",
           gestor_total_pendientes(g), gestor_total_completados(g));
    printf("--------------------------------------\n");
}

/* Trabajos completados, en el orden en que terminaron. */
static void mostrar_historial_impresion(const GestorImpresion *g)
{
    const Cola     *h;
    const NodoCola *nodo;
    int             posicion;

    h = gestor_historial(g);
    printf("\n--- HISTORIAL DE TRABAJOS COMPLETADOS ---\n");
    if (h == NULL || cola_esta_vacia(h)) {
        printf("  Todavia no se ha completado ningun trabajo.\n");
        printf("-----------------------------------------\n");
        return;
    }

    posicion = 1;
    for (nodo = h->frente; nodo != NULL; nodo = nodo->siguiente) {
        printf("  %d. #%d  %-20s %3d pag.  prioridad %-6s  %s\n",
               posicion,
               nodo->trabajo.id,
               nodo->trabajo.nombre,
               nodo->trabajo.paginas,
               gestor_nombre_prioridad(nodo->trabajo.prioridad),
               gestor_nombre_estado(nodo->trabajo.estado));
        posicion++;
    }
    printf("  Total completados: %d\n", gestor_total_completados(g));
    printf("-----------------------------------------\n");
}

/* --- Acciones del editor -------------------------------------------- */

/* Un documento nuevo arranca con su propio historial, asi que primero
 * se libera la pila anterior. */
static void accion_documento_nuevo(Editor *e)
{
    limpiarPila(&e->historial);
    inicializarEditor(e);
    printf("  [OK] Documento nuevo creado. El historial inicia vacio.\n");
}

static void accion_agregar(Editor *e)
{
    char entrada[MAX_LINEA];

    if (!leer_linea("  Texto a agregar: ", entrada, sizeof(entrada))) {
        return;
    }
    if (entrada[0] == '\0') {
        printf("  [!] No se agrego nada: el texto esta vacio.\n");
        return;
    }
    agregarTexto(e, entrada);
    printf("  [OK] Texto agregado. Estados en el historial: %d\n",
           (int) e->historial.tamanio);
}

static void accion_borrar_caracteres(Editor *e)
{
    int cantidad;

    size_t antes;

    if (!leer_entero("  Cuantos caracteres borrar del final: ", &cantidad)) {
        return;
    }

    antes = strlen(e->textoActual);
    borrarCaracteres(e, cantidad);
    if (strlen(e->textoActual) != antes) {
        printf("  [OK] Se borraron %d caracteres. Estados en el historial: %d\n",
               (int) (antes - strlen(e->textoActual)),
               (int) e->historial.tamanio);
    }
}

/* --- Acciones del gestor de impresion -------------------------------- */

/* Envia el documento actual a la cola del nivel indicado. El contenido
 * se entrega al gestor, que conserva su propia copia en el trabajo. */
static void accion_enviar(GestorImpresion *g, const Editor *e)
{
    char           nombre[COLA_MAX_NOMBRE];
    int            paginas;
    int            prioridad;
    Trabajo        registrado;
    ResultadoEnvio resultado;

    if (e->textoActual[0] == '\0') {
        printf("  [!] El documento esta vacio: nada que imprimir.\n");
        return;
    }

    if (!leer_linea("  Nombre del documento: ", nombre, sizeof(nombre))) {
        return;
    }
    if (!leer_entero("  Numero de paginas (> 0): ", &paginas)) {
        return;
    }
    if (!leer_entero("  Prioridad (1=Alta, 2=Normal, 3=Baja): ", &prioridad)) {
        return;
    }

    resultado = gestor_enviar(g, nombre, paginas, prioridad,
                              e->textoActual, &registrado);

    switch (resultado) {
        case ENVIO_OK:
            printf("  [OK] Trabajo #%d encolado en prioridad %s [%s].\n",
                   registrado.id, gestor_nombre_prioridad(registrado.prioridad),
                   gestor_nombre_estado(registrado.estado));
            printf("       La cola guarda una copia del texto (%d caracteres).\n",
                   (int) strlen(registrado.texto));
            break;
        case ENVIO_NOMBRE_INVALIDO:
            printf("  [!] El nombre del documento no puede estar vacio.\n");
            break;
        case ENVIO_PAGINAS_INVALIDAS:
            printf("  [!] El numero de paginas debe ser mayor que 0.\n");
            break;
        case ENVIO_PRIORIDAD_INVALIDA:
            printf("  [!] Prioridad invalida: solo se admite 1, 2 o 3.\n");
            break;
        case ENVIO_SIN_MEMORIA:
            printf("  [!] No hay memoria disponible para encolar el trabajo.\n");
            break;
    }
}

/* Toma el siguiente trabajo y lo lleva de principio a fin. */
static void accion_despachar(GestorImpresion *g)
{
    Trabajo           tomado;
    Trabajo           actual;
    Trabajo           terminado;
    ResultadoDespacho resultado;

    resultado = gestor_iniciar_siguiente(g, &tomado);
    switch (resultado) {
        case DESPACHO_INICIADO:
            printf("  [OK] Imprimiendo #%d %s (%d pag., prioridad %s) [%s].\n",
                   tomado.id, tomado.nombre, tomado.paginas,
                   gestor_nombre_prioridad(tomado.prioridad),
                   gestor_nombre_estado(tomado.estado));
            gestor_finalizar_en_curso(g, &terminado);
            printf("  [OK] Trabajo #%d %s -> %s. Agregado al historial.\n",
                   terminado.id, terminado.nombre,
                   gestor_nombre_estado(terminado.estado));
            break;
        case DESPACHO_OCUPADO:
            gestor_trabajo_en_curso(g, &actual);
            printf("  [i] La impresora esta ocupada con #%d %s.\n",
                   actual.id, actual.nombre);
            printf("      El trabajo en curso no se interrumpe.\n");
            break;
        case DESPACHO_SIN_TRABAJOS:
            printf("  [i] No hay trabajos pendientes en ninguna de las tres colas.\n");
            break;
    }
}

/* --- Programa principal ---------------------------------------------- */

int main(void)
{
    Editor          editor;
    GestorImpresion gestor;
    int             opcion;

    inicializarEditor(&editor);
    gestor_inicializar(&gestor);
    limpiarPantalla();

    for (;;) {
        mostrarMenu();
        if (!leer_entero("", &opcion)) {
            if (feof(stdin)) {
                break;
            }
            continue;
        }

        printf("\n");
        mostrarLineaSeparadora();

        switch (opcion) {
            case 1:
                accion_documento_nuevo(&editor);
                break;
            case 2:
                accion_agregar(&editor);
                break;
            case 3:
                accion_borrar_caracteres(&editor);
                break;
            case 4:
                borrarUltimaLinea(&editor);
                break;
            case 5:
                mostrarTexto(&editor);
                break;
            case 6:
                mostrarHistorial(&editor);
                break;
            case 7:
                deshacer(&editor);
                break;
            case 8:
                accion_enviar(&gestor, &editor);
                break;
            case 9:
                accion_despachar(&gestor);
                break;
            case 10:
                mostrar_estado(&gestor);
                break;
            case 11:
                mostrar_historial_impresion(&gestor);
                break;
            case 0:
                limpiarPila(&editor.historial);
                gestor_liberar(&gestor);
                printf("  Saliendo. Memoria liberada.\n");
                return EXIT_SUCCESS;
            default:
                printf("  [!] Opcion invalida.\n");
                break;
        }
    }

    limpiarPila(&editor.historial);
    gestor_liberar(&gestor);
    return EXIT_SUCCESS;
}
