/* Contador de reservas y liberaciones de memoria.
 * No se incluye desde ningun archivo: el Makefile lo inyecta con
 * -include solo en el objetivo "auditoria". */

#ifndef AUDITORIA_H
#define AUDITORIA_H

#include <stdlib.h>

void *auditoria_malloc(size_t n);
void  auditoria_free(void *p);

#define malloc(n) auditoria_malloc(n)
#define free(p)   auditoria_free(p)

#endif
