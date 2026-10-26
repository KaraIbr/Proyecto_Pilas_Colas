/* =====================================================================
 * impresion.c - Gestor de impresion: 3 colas de prioridad y despachador
 *
 * Proyecto Integrador Unidad 4: Pilas y Colas
 * Responsable: Aaron Salvador Castaneda Ruiz
 * ===================================================================== */

#include <stdlib.h>
#include <string.h>

#include "impresion.h"

/* Copia una cadena en un arreglo de destino dejando el fin de cadena
 * siempre colocado, incluso si el origen es mas largo que el destino. */
static void copiar_texto(char *destino, int tamano, const char *origen)
{
    if (destino == NULL || tamano <= 0) {
        return;
    }
    if (origen == NULL) {
        destino[0] = '\0';
        return;
    }
    strncpy(destino, origen, (size_t) (tamano - 1));
    destino[tamano - 1] = '\0';
}

/* Devuelve la cola interna (modificable) de un nivel de prioridad. */
static Cola *cola_del_nivel(GestorImpresion *g, int prioridad)
{
    if (g == NULL) {
        return NULL;
    }
    switch (prioridad) {
        case PRIORIDAD_ALTA:   return &g->alta;
        case PRIORIDAD_NORMAL: return &g->normal;
        case PRIORIDAD_BAJA:   return &g->baja;
        default:               return NULL;
    }
}

void gestor_inicializar(GestorImpresion *g)
{
    if (g == NULL) {
        return;
    }
    cola_inicializar(&g->alta);
    cola_inicializar(&g->normal);
    cola_inicializar(&g->baja);
    cola_inicializar(&g->historial);
    g->siguiente_id = 1;
    g->ocupada      = false;
    memset(&g->en_curso, 0, sizeof(g->en_curso));
}

void gestor_liberar(GestorImpresion *g)
{
    if (g == NULL) {
        return;
    }
    cola_vaciar(&g->alta);
    cola_vaciar(&g->normal);
    cola_vaciar(&g->baja);
    cola_vaciar(&g->historial);
    g->ocupada = false;
}

bool gestor_prioridad_valida(int prioridad)
{
    return (prioridad == PRIORIDAD_ALTA ||
            prioridad == PRIORIDAD_NORMAL ||
            prioridad == PRIORIDAD_BAJA);
}

bool gestor_paginas_validas(int paginas)
{
    return (paginas > 0);
}

ResultadoEnvio gestor_enviar(GestorImpresion *g,
                             const char *nombre,
                             int paginas,
                             int prioridad,
                             const char *texto,
                             Trabajo *registrado)
{
    Trabajo nuevo;
    Cola   *destino;

    if (g == NULL) {
        return ENVIO_NOMBRE_INVALIDO;
    }

    /* Validaciones de entrada antes de tocar la estructura. */
    if (nombre == NULL || nombre[0] == '\0') {
        return ENVIO_NOMBRE_INVALIDO;
    }
    if (!gestor_paginas_validas(paginas)) {
        return ENVIO_PAGINAS_INVALIDAS;
    }
    if (!gestor_prioridad_valida(prioridad)) {
        return ENVIO_PRIORIDAD_INVALIDA;
    }

    destino = cola_del_nivel(g, prioridad);
    if (destino == NULL) {
        return ENVIO_PRIORIDAD_INVALIDA;
    }

    /* Se arma el trabajo con una COPIA del nombre y del texto.
     * Al registrarlo queda En espera: todavia no lo atiende nadie. */
    nuevo.id            = g->siguiente_id;
    nuevo.orden_llegada = g->siguiente_id;
    nuevo.paginas       = paginas;
    nuevo.prioridad     = prioridad;
    nuevo.estado        = ESTADO_EN_ESPERA;
    copiar_texto(nuevo.nombre, COLA_MAX_NOMBRE, nombre);
    copiar_texto(nuevo.texto,  COLA_MAX_TEXTO,  texto);

    if (!cola_encolar(destino, &nuevo)) {
        return ENVIO_SIN_MEMORIA;
    }

    g->siguiente_id++;
    if (registrado != NULL) {
        *registrado = nuevo;
    }
    return ENVIO_OK;
}

bool gestor_hay_pendientes(const GestorImpresion *g)
{
    if (g == NULL) {
        return false;
    }
    return !cola_esta_vacia(&g->alta) ||
           !cola_esta_vacia(&g->normal) ||
           !cola_esta_vacia(&g->baja);
}

int gestor_total_pendientes(const GestorImpresion *g)
{
    if (g == NULL) {
        return 0;
    }
    return cola_cantidad(&g->alta) +
           cola_cantidad(&g->normal) +
           cola_cantidad(&g->baja);
}

const Cola *gestor_cola_de_prioridad(const GestorImpresion *g, int prioridad)
{
    if (g == NULL) {
        return NULL;
    }
    switch (prioridad) {
        case PRIORIDAD_ALTA:   return &g->alta;
        case PRIORIDAD_NORMAL: return &g->normal;
        case PRIORIDAD_BAJA:   return &g->baja;
        default:               return NULL;
    }
}

bool gestor_trabajo_en_curso(const GestorImpresion *g, Trabajo *salida)
{
    if (g == NULL || salida == NULL || !g->ocupada) {
        return false;
    }
    *salida = g->en_curso;
    return true;
}

ResultadoDespacho gestor_iniciar_siguiente(GestorImpresion *g, Trabajo *salida)
{
    Trabajo tomado;

    if (g == NULL) {
        return DESPACHO_SIN_TRABAJOS;
    }

    /* Politica no expropiativa: un trabajo en curso jamas se interrumpe,
     * aunque haya llegado algo de prioridad Alta despues. */
    if (g->ocupada) {
        return DESPACHO_OCUPADO;
    }

    /* Orden de atencion: Alta, luego Normal, luego Baja.
     * cola_desencolar respeta el FIFO interno de cada nivel. */
    if (cola_desencolar(&g->alta, &tomado) ||
        cola_desencolar(&g->normal, &tomado) ||
        cola_desencolar(&g->baja, &tomado)) {

        /* Al salir de su cola el trabajo pasa a Procesando. */
        tomado.estado = ESTADO_PROCESANDO;
        g->en_curso   = tomado;
        g->ocupada    = true;
        if (salida != NULL) {
            *salida = tomado;
        }
        return DESPACHO_INICIADO;
    }

    /* Las tres colas estan vacias: no es un error, solo no hay trabajo. */
    return DESPACHO_SIN_TRABAJOS;
}

bool gestor_finalizar_en_curso(GestorImpresion *g, Trabajo *salida)
{
    Trabajo terminado;

    if (g == NULL || !g->ocupada) {
        return false;
    }

    /* El trabajo pasa a Completado y se registra en el historial. */
    terminado        = g->en_curso;
    terminado.estado = ESTADO_COMPLETADO;
    cola_encolar(&g->historial, &terminado);

    if (salida != NULL) {
        *salida = terminado;
    }
    g->ocupada = false;
    memset(&g->en_curso, 0, sizeof(g->en_curso));
    return true;
}

bool gestor_siguiente_por_atender(const GestorImpresion *g, Trabajo *salida)
{
    if (g == NULL || salida == NULL) {
        return false;
    }
    /* Mismo orden que usa el despachador, pero sin retirar nada. */
    return cola_consultar_frente(&g->alta, salida) ||
           cola_consultar_frente(&g->normal, salida) ||
           cola_consultar_frente(&g->baja, salida);
}

const Cola *gestor_historial(const GestorImpresion *g)
{
    return (g == NULL) ? NULL : &g->historial;
}

int gestor_total_completados(const GestorImpresion *g)
{
    return (g == NULL) ? 0 : cola_cantidad(&g->historial);
}

const char *gestor_nombre_estado(EstadoTrabajo estado)
{
    switch (estado) {
        case ESTADO_EN_ESPERA:  return "En espera";
        case ESTADO_PROCESANDO: return "Procesando";
        case ESTADO_COMPLETADO: return "Completado";
        default:                return "Desconocido";
    }
}

const char *gestor_nombre_prioridad(int prioridad)
{
    switch (prioridad) {
        case PRIORIDAD_ALTA:   return "ALTA";
        case PRIORIDAD_NORMAL: return "NORMAL";
        case PRIORIDAD_BAJA:   return "BAJA";
        default:               return "INVALIDA";
    }
}
