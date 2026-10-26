/* =====================================================================
 * impresion.h - Gestor de impresion con 3 colas de prioridad
 *
 * Proyecto Integrador Unidad 4: Pilas y Colas
 * Responsable: Aaron Salvador Castaneda Ruiz
 *
 * Diseno: TRES colas FIFO independientes, una por nivel de prioridad.
 *
 *      ALTA   (1)  FRENTE -> [ ] -> [ ] <- FINAL
 *      NORMAL (2)  FRENTE -> [ ] -> [ ] <- FINAL
 *      BAJA   (3)  FRENTE -> [ ] -> [ ] <- FINAL
 *
 * El despachador siempre atiende la cola Alta hasta agotarla, luego
 * Normal y al final Baja; dentro de cada nivel el orden es FIFO
 * estricto. Esta separacion es preferible a mantener una sola lista
 * ordenada porque:
 *   - encolar es O(1) y no hay que buscar la posicion de insercion;
 *   - el orden de llegada dentro de un nivel nunca se altera;
 *   - la prioridad es una propiedad de la estructura, no del dato.
 *
 * Politica NO EXPROPIATIVA: mientras un trabajo esta en curso, la
 * llegada de un trabajo de prioridad Alta no lo interrumpe; el nuevo
 * trabajo solo se considera cuando el actual termina.
 * ===================================================================== */

#ifndef IMPRESION_H
#define IMPRESION_H

#include <stdbool.h>

#include "cola.h"

/* Niveles de prioridad validos */
#define PRIORIDAD_ALTA   1
#define PRIORIDAD_NORMAL 2
#define PRIORIDAD_BAJA   3

/* Resultado de intentar enviar un documento a la cola de impresion */
typedef enum {
    ENVIO_OK = 0,
    ENVIO_NOMBRE_INVALIDO,      /* nombre vacio                      */
    ENVIO_PAGINAS_INVALIDAS,    /* paginas <= 0                      */
    ENVIO_PRIORIDAD_INVALIDA,   /* prioridad fuera de {1, 2, 3}      */
    ENVIO_SIN_MEMORIA           /* fallo malloc al encolar           */
} ResultadoEnvio;

/* Resultado de pedirle al despachador que inicie el siguiente trabajo */
typedef enum {
    DESPACHO_INICIADO = 0,      /* se tomo un trabajo y esta en curso */
    DESPACHO_OCUPADO,           /* ya hay un trabajo en curso         */
    DESPACHO_SIN_TRABAJOS       /* las tres colas estan vacias        */
} ResultadoDespacho;

/* Estado completo del gestor de impresion */
typedef struct {
    Cola    alta;           /* prioridad 1 */
    Cola    normal;         /* prioridad 2 */
    Cola    baja;           /* prioridad 3 */
    Cola    historial;      /* trabajos completados, en orden de termino */
    int     siguiente_id;   /* folio consecutivo para los trabajos */
    bool    ocupada;        /* true si hay un trabajo en curso     */
    Trabajo en_curso;       /* trabajo que la impresora atiende    */
} GestorImpresion;

/* --- Ciclo de vida ------------------------------------------------- */

/* Inicializa las tres colas y el estado de la impresora. */
void gestor_inicializar(GestorImpresion *g);

/* Libera toda la memoria de las tres colas. */
void gestor_liberar(GestorImpresion *g);

/* --- Validaciones -------------------------------------------------- */

/* true solo si prioridad pertenece a {1, 2, 3}. */
bool gestor_prioridad_valida(int prioridad);

/* true solo si paginas > 0. */
bool gestor_paginas_validas(int paginas);

/* --- Envio de trabajos --------------------------------------------- */

/* Valida los datos, asigna folio y encola una COPIA del documento en
 * la cola del nivel correspondiente. Si 'registrado' no es NULL, ahi
 * se devuelve el trabajo tal como quedo encolado. */
ResultadoEnvio gestor_enviar(GestorImpresion *g,
                             const char *nombre,
                             int paginas,
                             int prioridad,
                             const char *texto,
                             Trabajo *registrado);

/* --- Consultas ----------------------------------------------------- */

/* true si queda al menos un trabajo pendiente en cualquiera de las tres colas. */
bool gestor_hay_pendientes(const GestorImpresion *g);

/* Suma de los pendientes de las tres colas. */
int gestor_total_pendientes(const GestorImpresion *g);

/* Cola asociada a un nivel de prioridad, o NULL si el nivel no existe.
 * Pensada para que la capa de interfaz pueda mostrar los pendientes
 * sin modificar la estructura. */
const Cola *gestor_cola_de_prioridad(const GestorImpresion *g, int prioridad);

/* Copia en *salida el trabajo en curso. false si la impresora esta libre. */
bool gestor_trabajo_en_curso(const GestorImpresion *g, Trabajo *salida);

/* Copia en *salida el siguiente trabajo que se atenderia (sin retirarlo de su
 * cola). false si no hay pendientes. */
bool gestor_siguiente_por_atender(const GestorImpresion *g, Trabajo *salida);

/* --- Historial de trabajos completados ------------------------------ */

/* Cola con los trabajos completados, del primero al ultimo en terminar. */
const Cola *gestor_historial(const GestorImpresion *g);

/* Cantidad de trabajos completados. */
int gestor_total_completados(const GestorImpresion *g);

/* --- Despachador --------------------------------------------------- */

/* Toma el siguiente trabajo segun la politica Alta > Normal > Baja
 * (FIFO dentro de cada nivel) y lo marca como en curso.
 * No interrumpe un trabajo ya en curso: devuelve DESPACHO_OCUPADO. */
ResultadoDespacho gestor_iniciar_siguiente(GestorImpresion *g, Trabajo *salida);

/* Marca como terminado el trabajo en curso y libera la impresora.
 * false si no habia ningun trabajo en curso. */
bool gestor_finalizar_en_curso(GestorImpresion *g, Trabajo *salida);

/* Nombre legible de un nivel de prioridad ("ALTA", "NORMAL", "BAJA"). */
const char *gestor_nombre_prioridad(int prioridad);

/* Nombre legible de un estado ("En espera", "Procesando", "Completado"). */
const char *gestor_nombre_estado(EstadoTrabajo estado);

#endif /* IMPRESION_H */
