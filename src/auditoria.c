/* Cuenta las reservas y liberaciones e informa al terminar el programa.
 * Este archivo se compila sin -include, de modo que malloc y free
 * siguen siendo los de la biblioteca estandar. */

#include <stdio.h>
#include <stdlib.h>

static long reservas = 0;
static long liberaciones = 0;
static int  registrado = 0;

static void informar(void)
{
    printf("\n[MEMORIA] reservas=%ld liberaciones=%ld vivas=%ld\n",
           reservas, liberaciones, reservas - liberaciones);
}

void *auditoria_malloc(size_t n)
{
    if (!registrado) {
        registrado = 1;
        atexit(informar);
    }
    reservas++;
    return malloc(n);
}

void auditoria_free(void *p)
{
    if (p != NULL) {
        liberaciones++;
    }
    free(p);
}
