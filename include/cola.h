/* =====================================================================
 * cola.h - Estructura de datos Cola (FIFO) para trabajos de impresion
 *
 * Proyecto Integrador Unidad 4: Pilas y Colas
 * Responsable: Aaron Salvador Castaneda Ruiz
 *
 * La cola se implementa con lista enlazada simple y DOS punteros
 * explicitos:
 *
 *      FRENTE  ->  [T1] -> [T2] -> [T3] <- FINAL
 *                   ^                 ^
 *                   |                 |
 *            se desencola aqui   se encola aqui
 *
 * Esto garantiza la disciplina FIFO (First In, First Out) con costo
 * O(1) tanto al encolar como al desencolar: el primero que entra es
 * siempre el primero que sale, y nunca se recorre la estructura para
 * decidir el orden de atencion.
 * ===================================================================== */

#ifndef COLA_H
#define COLA_H

#include <stdbool.h>

/* Limites de los campos de un trabajo de impresion */
#define COLA_MAX_NOMBRE 64
/* El contenido del documento no vive aqui: se guarda en trabajo_NN.txt
 * al enviarlo, y de ahi lo recupera la impresora. */

/* Estado en que puede encontrarse un trabajo de impresion */
typedef enum {
    ESTADO_EN_ESPERA = 0,   /* registrado y esperando turno en su cola */
    ESTADO_PROCESANDO,      /* la impresora lo esta atendiendo        */
    ESTADO_COMPLETADO       /* termino y paso al historial            */
} EstadoTrabajo;

/* ---------------------------------------------------------------------
 * Trabajo de impresion.
 *
 * El trabajo guarda solo los datos de control. El contenido del
 * documento vive en el archivo trabajo_NN.txt, escrito en el momento
 * del envio: como no se vuelve a tocar, conserva el texto de entonces
 * aunque el documento se edite despues.
 * ------------------------------------------------------------------- */
typedef struct {
    int           id;                      /* clave unica del trabajo              */
    char          nombre[COLA_MAX_NOMBRE]; /* nombre del documento                 */
    int           paginas;                 /* numero de paginas (siempre > 0)      */
    int           prioridad;               /* 1 = Alta, 2 = Normal, 3 = Baja       */
    int           orden_llegada;           /* consecutivo asignado al registrarlo  */
    EstadoTrabajo estado;                  /* en espera, procesando o completado   */
} Trabajo;

/* Nodo de la lista enlazada que sostiene la cola */
typedef struct NodoCola {
    Trabajo          trabajo;
    struct NodoCola *siguiente;
} NodoCola;

/* Cola FIFO con punteros explicitos a FRENTE y FINAL */
typedef struct {
    NodoCola *frente;   /* por donde salen los trabajos */
    NodoCola *final;    /* por donde entran los trabajos */
    int       cantidad; /* numero de trabajos pendientes */
} Cola;

/* --- Operaciones primitivas de la cola ----------------------------- */

/* Deja la cola en estado vacio valido. Debe llamarse antes de usarla. */
void cola_inicializar(Cola *c);

/* Validacion de cola vacia: true si no hay ningun trabajo pendiente. */
bool cola_esta_vacia(const Cola *c);

/* Numero de trabajos pendientes en la cola. */
int cola_cantidad(const Cola *c);

/* ENQUEUE: inserta una copia de *t al FINAL de la cola.
 * Devuelve false si falla la reserva de memoria o si los punteros
 * recibidos no son validos. */
bool cola_encolar(Cola *c, const Trabajo *t);

/* DEQUEUE: retira el trabajo del FRENTE y lo copia en *salida.
 * Devuelve false si la cola esta vacia (no es un error fatal: el
 * llamador debe informar al usuario y continuar). */
bool cola_desencolar(Cola *c, Trabajo *salida);

/* Consulta el trabajo del FRENTE sin retirarlo (no modifica la cola).
 * Devuelve false si la cola esta vacia. */
bool cola_consultar_frente(const Cola *c, Trabajo *salida);

/* Libera todos los nodos y deja la cola vacia. */
void cola_vaciar(Cola *c);

#endif /* COLA_H */
