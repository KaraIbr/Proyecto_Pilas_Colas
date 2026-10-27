# Aportes — Aarón Salvador Castañeda Ruiz

**Rama:** `feature/gestor-impresion`
**Módulo:** Gestor de Impresión (colas FIFO con prioridades)

## Archivos que hice

| Archivo | Qué contiene |
|---|---|
| `include/cola.h` | Interfaz de la cola FIFO |
| `src/cola.c` | Encolar, desencolar, consultar frente, cola vacía |
| `include/impresion.h` | Interfaz del gestor con las 3 colas |
| `src/impresion.c` | Las 3 colas de prioridad y el despachador |
| `src/main.c` | Menú y envío del documento a la cola |
| `src/pruebas_impresion.c` | Batería de pruebas (28 verificaciones) |
| `docs/pseudocodigo-cola.md` | Pseudocódigo y diagramas de la cola |

## Lo que me toca defender

**Por qué tres colas y no una lista ordenada.** Con tres colas separadas,
encolar es directo al final sin buscar posición, y el orden de llegada dentro
de un nivel no se puede alterar. Una lista ordenada habría que recorrerla en cada inserción.

**Por qué no expropiativo.** El despachador revisa primero si hay un trabajo en curso y, si lo hay, no toca las colas. Un trabajo urgente espera a que termine el actual, igual que una impresora real: una hoja a medio imprimir no se devuelve.

**Por qué el contenido vive en un archivo.** El trabajo guarda solo los datos de control; el documento se escribe en `trabajo_NN.txt` al enviarlo y se lee de ahí al imprimirlo. Como el archivo se escribe una sola vez, conserva el texto del momento del envío aunque el documento se edite después (caso 8).

## Pruebas

28 de 28 verificaciones pasan. Detalle en
[`evidencias_gestor_impresion.md`](evidencias_gestor_impresion.md).
Casos de la rúbrica cubiertos: 1, 2, 3, 4 y 8.

## Pendientes

- Capturas de pantalla para el reporte.
- Conectar `main.c` con el editor de Karina (el punto está marcado en el
  código como `[SEAM EDITOR]`).
- Instalar un MinGW moderno: el de Dev-C++ no enlaza.
