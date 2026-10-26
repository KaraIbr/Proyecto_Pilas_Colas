# Evidencias de Pruebas

**Responsable del gestor de impresión:** Aarón Salvador Castañeda Ruiz

## Cómo ejecutar

```bash
make correr-pruebas    # pruebas del gestor de impresion (44)
make pruebas-editor    # pruebas del editor y la pila (18)
make                   # programa interactivo
```

## Resultados

| Suite | Verificaciones |
|---|---|
| Gestor de impresión | 44 de 44 |
| Editor y pila | 18 de 18 |

Salidas completas en [`salida_pruebas_impresion.txt`](salida_pruebas_impresion.txt),
[`salida_pruebas_editor.txt`](salida_pruebas_editor.txt) y
[`salida_simulador.txt`](salida_simulador.txt).

## Casos obligatorios (sección 7 de la rúbrica)

| # | Caso | Estado |
|---|---|---|
| 1 | Misma prioridad, se respeta FIFO | PASA |
| 2 | Prioridades diferentes | PASA |
| 3 | Urgente va al final de su nivel y no interrumpe | PASA |
| 4 | Procesar sin trabajos pendientes | PASA |
| 5 | Varias modificaciones conservan los estados | PASA |
| 6 | Deshacer varias veces, orden LIFO | PASA |
| 7 | Deshacer con pila vacía | PASA |
| 8 | Documento editado tras enviarlo | PASA |

## Otros requisitos verificados

- Campo **Estado**: En espera → Procesando → Completado (sección 3.1).
- **Historial** de trabajos completados y su consulta (sección 3.3).
- **Siguiente trabajo por atender** visible en pantalla (sección 3.3).
- Validaciones: rechaza páginas ≤ 0 y prioridades fuera de 1-3.
- Secuencia ilustrativa del editor (4.2): Red → Red segura → borrar 7 → deshacer.

## Caso del simulador (sección 3.4)

Con los datos exactos de la rúbrica (A 5 Normal, B 2 Alta, C 10 Normal,
D 3 Alta, E 8 Baja), el orden de salida es **B, D, A, C, E**.

`salida_simulador.txt` muestra el estado inicial de las tres colas, cada
despacho y el historial final con los cinco trabajos completados.

## Pendiente

- Capturas de pantalla para el reporte PDF.
- El GCC de Dev-C++ (3.4.2) no enlaza en Windows 11. Hay que instalar un
  MinGW moderno antes de la presentación.
