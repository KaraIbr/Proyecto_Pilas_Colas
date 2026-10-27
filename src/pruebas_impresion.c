/* =====================================================================
 * pruebas_impresion.c - Bateria de pruebas del Gestor de Impresion
 *
 * Proyecto Integrador Unidad 4: Pilas y Colas
 * Responsable: Aaron Salvador Castaneda Ruiz
 *
 * Cubre los casos obligatorios que corresponden al
 * modulo de impresion:
 *
 *   Caso 1 - Multiples trabajos con la misma prioridad (FIFO estricto)
 *   Caso 2 - Trabajos con prioridades diferentes (Alta > Normal > Baja)
 *   Caso 3 - Llegada de trabajo urgente durante proceso (no expropiativo)
 *   Caso 4 - Intento de procesar sin trabajos pendientes
 *   Caso 8 - Copia inmutable del documento al encolarlo (lado cola)
 *
 * Mas la secuencia del simulador y las validaciones de entrada.
 *
 * Compilar y ejecutar:  make correr-pruebas
 * ===================================================================== */

#include <stdio.h>
#include <string.h>

#include "cola.h"
#include "impresion.h"

static int pruebas_totales = 0;
static int pruebas_ok      = 0;

/* Registra el resultado de una verificacion individual. */
static void verificar(const char *descripcion, bool condicion)
{
    pruebas_totales++;
    if (condicion) {
        pruebas_ok++;
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

/* Despacha un trabajo completo (iniciar + finalizar) y devuelve su
 * nombre en 'nombre_salida'. false si no habia trabajos. */
static bool despachar_uno(GestorImpresion *g, char *nombre_salida, int tamano)
{
    Trabajo t;

    if (gestor_iniciar_siguiente(g, &t) != DESPACHO_INICIADO) {
        return false;
    }
    strncpy(nombre_salida, t.nombre, (size_t) (tamano - 1));
    nombre_salida[tamano - 1] = '\0';
    gestor_finalizar_en_curso(g, NULL);
    return true;
}

/* === CASO 1: Multiples trabajos con la misma prioridad ============== */
static void caso_1_misma_prioridad(void)
{
    GestorImpresion g;
    char            nombre[COLA_MAX_NOMBRE];

    titulo("CASO 1: Multiples trabajos con la misma prioridad (FIFO)");
    gestor_inicializar(&g);

    gestor_enviar(&g, "Doc-1", 3, PRIORIDAD_NORMAL, NULL);
    gestor_enviar(&g, "Doc-2", 1, PRIORIDAD_NORMAL, NULL);
    gestor_enviar(&g, "Doc-3", 7, PRIORIDAD_NORMAL, NULL);
    printf("    Encolados en NORMAL, en este orden: Doc-1, Doc-2, Doc-3\n");

    verificar("quedan 3 trabajos pendientes", gestor_total_pendientes(&g) == 3);

    despachar_uno(&g, nombre, sizeof(nombre));
    verificar("sale primero Doc-1 (el que llego primero)", strcmp(nombre, "Doc-1") == 0);
    despachar_uno(&g, nombre, sizeof(nombre));
    verificar("sale despues Doc-2", strcmp(nombre, "Doc-2") == 0);
    despachar_uno(&g, nombre, sizeof(nombre));
    verificar("sale al final Doc-3", strcmp(nombre, "Doc-3") == 0);
    verificar("la cola queda vacia", !gestor_hay_pendientes(&g));

    gestor_liberar(&g);
}

/* === CASO 2: Trabajos con prioridades diferentes ==================== */
static void caso_2_prioridades_distintas(void)
{
    GestorImpresion g;
    char            nombre[COLA_MAX_NOMBRE];

    titulo("CASO 2: Trabajos con prioridades diferentes");
    gestor_inicializar(&g);

    /* Se encolan a proposito en orden inverso a su prioridad. */
    gestor_enviar(&g, "Baja-1",   2, PRIORIDAD_BAJA, NULL);
    gestor_enviar(&g, "Normal-1", 2, PRIORIDAD_NORMAL, NULL);
    gestor_enviar(&g, "Alta-1",   2, PRIORIDAD_ALTA, NULL);
    printf("    Encolados en este orden: Baja-1, Normal-1, Alta-1\n");

    despachar_uno(&g, nombre, sizeof(nombre));
    verificar("Alta precede a todos aunque llego ultimo", strcmp(nombre, "Alta-1") == 0);
    despachar_uno(&g, nombre, sizeof(nombre));
    verificar("Normal se atiende antes que Baja", strcmp(nombre, "Normal-1") == 0);
    despachar_uno(&g, nombre, sizeof(nombre));
    verificar("Baja se atiende al final", strcmp(nombre, "Baja-1") == 0);

    gestor_liberar(&g);
}

/* === CASO 3: Llegada de trabajo urgente durante proceso ============= */
static void caso_3_urgente_durante_proceso(void)
{
    GestorImpresion g;
    Trabajo         en_curso;
    Trabajo         tomado;

    titulo("CASO 3: Trabajo urgente llega durante un proceso activo");
    gestor_inicializar(&g);

    gestor_enviar(&g, "Informe-Baja", 10, PRIORIDAD_BAJA, NULL);
    gestor_iniciar_siguiente(&g, &en_curso);
    printf("    En curso: %s (prioridad %s)\n", en_curso.nombre,
           gestor_nombre_prioridad(en_curso.prioridad));

    /* Ya habia un trabajo Alta esperando antes del urgente. */
    gestor_enviar(&g, "Alta-Previa", 2, PRIORIDAD_ALTA, NULL);
    printf("    Pendiente en ALTA: Alta-Previa\n");

    /* Llega un trabajo urgente mientras el otro se imprime. */
    gestor_enviar(&g, "Urgente-Alta", 1, PRIORIDAD_ALTA, NULL);
    printf("    Llega: Urgente-Alta (prioridad ALTA)\n");

    verificar("el despachador reporta OCUPADO, no interrumpe",
              gestor_iniciar_siguiente(&g, &tomado) == DESPACHO_OCUPADO);
    gestor_trabajo_en_curso(&g, &en_curso);
    verificar("el trabajo en curso sigue siendo Informe-Baja",
              strcmp(en_curso.nombre, "Informe-Baja") == 0);
    verificar("ambos Alta permanecen pendientes", gestor_total_pendientes(&g) == 2);

    /* Se coloca AL FINAL de su nivel: Alta-Previa sigue al frente. */
    gestor_siguiente_por_atender(&g, &tomado);
    verificar("el urgente se coloco al final de su nivel, no al frente",
              strcmp(tomado.nombre, "Alta-Previa") == 0);

    /* Al terminar el actual, se respeta el FIFO dentro de ALTA. */
    gestor_finalizar_en_curso(&g, NULL);
    verificar("al liberarse la impresora entra Alta-Previa",
              gestor_iniciar_siguiente(&g, &tomado) == DESPACHO_INICIADO &&
              strcmp(tomado.nombre, "Alta-Previa") == 0);
    gestor_finalizar_en_curso(&g, NULL);
    verificar("y despues el urgente",
              gestor_iniciar_siguiente(&g, &tomado) == DESPACHO_INICIADO &&
              strcmp(tomado.nombre, "Urgente-Alta") == 0);

    gestor_liberar(&g);
}

/* === CASO 4: Intento de procesar sin trabajos pendientes ============ */
static void caso_4_sin_trabajos(void)
{
    GestorImpresion g;
    Trabajo         tomado;

    titulo("CASO 4: Procesar con las tres colas vacias");
    gestor_inicializar(&g);

    verificar("no hay pendientes al iniciar", !gestor_hay_pendientes(&g));
    verificar("el despachador informa SIN_TRABAJOS (sin error fatal)",
              gestor_iniciar_siguiente(&g, &tomado) == DESPACHO_SIN_TRABAJOS);
    verificar("la impresora sigue libre", !gestor_trabajo_en_curso(&g, &tomado));
    verificar("finalizar sin trabajo en curso se rechaza sin fallo",
              gestor_finalizar_en_curso(&g, NULL) == false);
    verificar("el gestor sigue operativo despues del intento",
              gestor_enviar(&g, "Posterior", 1, PRIORIDAD_NORMAL, NULL) == ENVIO_OK);

    gestor_liberar(&g);
}

/* === Validaciones de entrada ======================================= */
static void caso_validaciones(void)
{
    GestorImpresion g;

    titulo("VALIDACIONES: paginas > 0 y prioridad en {1, 2, 3}");
    gestor_inicializar(&g);

    verificar("rechaza 0 paginas",
              gestor_enviar(&g, "X", 0, PRIORIDAD_ALTA, NULL) == ENVIO_PAGINAS_INVALIDAS);
    verificar("rechaza paginas negativas",
              gestor_enviar(&g, "X", -5, PRIORIDAD_ALTA, NULL) == ENVIO_PAGINAS_INVALIDAS);
    verificar("rechaza prioridad 0",
              gestor_enviar(&g, "X", 1, 0, NULL) == ENVIO_PRIORIDAD_INVALIDA);
    verificar("rechaza prioridad 4",
              gestor_enviar(&g, "X", 1, 4, NULL) == ENVIO_PRIORIDAD_INVALIDA);
    verificar("rechaza prioridad negativa",
              gestor_enviar(&g, "X", 1, -1, NULL) == ENVIO_PRIORIDAD_INVALIDA);
    verificar("rechaza nombre vacio",
              gestor_enviar(&g, "", 1, PRIORIDAD_ALTA, NULL) == ENVIO_NOMBRE_INVALIDO);
    verificar("acepta los tres niveles validos",
              gestor_enviar(&g, "A", 1, PRIORIDAD_ALTA, NULL) == ENVIO_OK &&
              gestor_enviar(&g, "B", 1, PRIORIDAD_NORMAL, NULL) == ENVIO_OK &&
              gestor_enviar(&g, "C", 1, PRIORIDAD_BAJA, NULL) == ENVIO_OK);
    verificar("ningun envio invalido entro a las colas",
              gestor_total_pendientes(&g) == 3);

    gestor_liberar(&g);
}

/* === Secuencia del simulador =======================================
 * Llegada | Trabajo     | Paginas | Prioridad
 *    1    | Documento A |    5    | Normal
 *    2    | Documento B |    2    | Alta
 *    3    | Documento C |   10    | Normal
 *    4    | Documento D |    3    | Alta
 *    5    | Documento E |    8    | Baja
 * Salida esperada si todos estan pendientes: B, D, A, C, E
 * ==================================================================== */
static void caso_simulador(void)
{
    GestorImpresion g;
    char            nombre[COLA_MAX_NOMBRE];
    char            obtenido[128];
    bool            primero = true;

    titulo("SIMULADOR: salida esperada B, D, A, C, E");
    gestor_inicializar(&g);

    gestor_enviar(&g, "Documento A",  5, PRIORIDAD_NORMAL, NULL);
    gestor_enviar(&g, "Documento B",  2, PRIORIDAD_ALTA, NULL);
    gestor_enviar(&g, "Documento C", 10, PRIORIDAD_NORMAL, NULL);
    gestor_enviar(&g, "Documento D",  3, PRIORIDAD_ALTA, NULL);
    gestor_enviar(&g, "Documento E",  8, PRIORIDAD_BAJA, NULL);

    printf("    Llegada: A(5,Normal) B(2,Alta) C(10,Normal) D(3,Alta) E(8,Baja)\n");
    printf("    Estado inicial de las colas:\n");
    printf("       ALTA:   Documento B -> Documento D\n");
    printf("       NORMAL: Documento A -> Documento C\n");
    printf("       BAJA:   Documento E\n");

    verificar("los 5 trabajos quedan pendientes", gestor_total_pendientes(&g) == 5);

    obtenido[0] = '\0';
    while (despachar_uno(&g, nombre, sizeof(nombre))) {
        if (!primero) {
            strcat(obtenido, ", ");
        }
        strcat(obtenido, nombre);
        primero = false;
    }

    printf("    Orden de despacho obtenido: %s\n", obtenido);
    verificar("el orden de salida es B, D, A, C, E",
              strcmp(obtenido, "Documento B, Documento D, Documento A, "
                               "Documento C, Documento E") == 0);
    verificar("las colas quedan vacias", !gestor_hay_pendientes(&g));
    verificar("los 5 quedaron en el historial", gestor_total_completados(&g) == 5);

    gestor_liberar(&g);
}

/* === Campo Estado del trabajo ====================================== */
static void caso_estados(void)
{
    GestorImpresion g;
    Trabajo         t;

    titulo("ESTADO: En espera -> Procesando -> Completado");
    gestor_inicializar(&g);

    gestor_enviar(&g, "Informe", 4, PRIORIDAD_NORMAL, &t);
    verificar("al registrarlo queda En espera", t.estado == ESTADO_EN_ESPERA);
    verificar("se le asigna orden de llegada", t.orden_llegada == 1);

    gestor_iniciar_siguiente(&g, &t);
    verificar("al despacharlo pasa a Procesando", t.estado == ESTADO_PROCESANDO);
    gestor_trabajo_en_curso(&g, &t);
    verificar("el trabajo en curso reporta Procesando", t.estado == ESTADO_PROCESANDO);

    gestor_finalizar_en_curso(&g, &t);
    verificar("al terminar pasa a Completado", t.estado == ESTADO_COMPLETADO);

    gestor_liberar(&g);
}

/* === Historial de trabajos completados ============================= */
static void caso_historial(void)
{
    GestorImpresion g;
    const Cola     *h;
    Trabajo         primero_hist;
    char            nombre[COLA_MAX_NOMBRE];

    titulo("HISTORIAL: los trabajos completados se conservan en orden");
    gestor_inicializar(&g);

    verificar("el historial arranca vacio", gestor_total_completados(&g) == 0);

    gestor_enviar(&g, "Uno", 1, PRIORIDAD_BAJA, NULL);
    gestor_enviar(&g, "Dos", 1, PRIORIDAD_ALTA, NULL);
    gestor_enviar(&g, "Tres", 1, PRIORIDAD_NORMAL, NULL);

    despachar_uno(&g, nombre, sizeof(nombre));   /* Dos  (Alta)   */
    despachar_uno(&g, nombre, sizeof(nombre));   /* Tres (Normal) */
    despachar_uno(&g, nombre, sizeof(nombre));   /* Uno  (Baja)   */

    verificar("quedan 3 trabajos completados", gestor_total_completados(&g) == 3);

    h = gestor_historial(&g);
    verificar("el historial no esta vacio", h != NULL && !cola_esta_vacia(h));
    cola_consultar_frente(h, &primero_hist);
    printf("    Primero del historial: %s\n", primero_hist.nombre);
    verificar("el primero del historial es el primero en terminar",
              strcmp(primero_hist.nombre, "Dos") == 0);
    verificar("los del historial estan marcados Completado",
              primero_hist.estado == ESTADO_COMPLETADO);
    verificar("consultar el historial no lo modifica",
              gestor_total_completados(&g) == 3);

    gestor_liberar(&g);
}

int main(void)
{
    printf("========================================================\n");
    printf(" BATERIA DE PRUEBAS - GESTOR DE IMPRESION (Pilas y Colas)\n");
    printf(" Modulo: src/cola.c + src/impresion.c\n");
    printf("========================================================\n");

    caso_1_misma_prioridad();
    caso_2_prioridades_distintas();
    caso_3_urgente_durante_proceso();
    caso_4_sin_trabajos();
    caso_estados();
    caso_historial();
    caso_validaciones();
    caso_simulador();

    printf("\n========================================================\n");
    printf(" RESULTADO: %d de %d verificaciones pasaron\n", pruebas_ok, pruebas_totales);
    printf("========================================================\n");

    return (pruebas_ok == pruebas_totales) ? 0 : 1;
}
