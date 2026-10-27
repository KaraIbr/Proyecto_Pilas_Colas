# Guía de ejecución

## Requisitos

GCC con soporte C99 y `make`. En Windows, MinGW-w64.

## Compilar

Desde la raíz del proyecto:

```bash
make
```

Genera `bin/editor_impresion`.

## Ejecutar

```bash
./bin/editor_impresion
```

En Windows, desde PowerShell:

```powershell
.\bin\editor_impresion.exe
```

## El menú

| Opción | Qué hace |
|---|---|
| 1 | Crear documento nuevo (vacía también el historial) |
| 2 | Agregar texto al final |
| 3 | Eliminar una cantidad de caracteres del final |
| 4 | Borrar la última línea |
| 5 | Mostrar el texto actual |
| 6 | Mostrar la pila de historial |
| 7 | Deshacer |
| 8 | Enviar el documento a impresión |
| 9 | Iniciar el siguiente trabajo |
| 10 | Finalizar el trabajo en curso |
| 11 | Ver el estado de las tres colas |
| 12 | Ver el historial de impresión |
| 13 | Guardar el documento en un archivo |
| 0 | Salir |

Se escribe una opción por línea. Cuando una opción pide datos, los va
preguntando de uno en uno.

## Dónde se guardan los archivos

Al enviar un documento a impresión (opción 8) se escribe un archivo
`trabajo_NN.txt`, donde `NN` es el folio del trabajo. **Ese archivo es el
contenido que se imprime:** la cola solo guarda los datos de control.

Los archivos se crean en el directorio desde el que se lanzó el programa.
Si se ejecuta desde la raíz del proyecto, quedan ahí. Están ignorados en
`.gitignore`.

La opción 13 guarda el documento en la ruta que se indique, sin añadir
extensión: lo que se escriba es el nombre del archivo.

## Flujo de ejemplo

Enviar un documento, editarlo después e imprimirlo. Sirve para ver que
el trabajo encolado no cambia aunque el documento sí.

```
2
Contrato version uno
8
Contrato
2
2
```

Responde `Documento guardado en trabajo_01.txt (20 caracteres).`

```
2
 Y EDICION POSTERIOR
5
```

El documento ahora dice `Contrato version uno Y EDICION POSTERIOR`.

```
9
```

```
[OK] Imprimiendo #1 Contrato (2 pag., prioridad NORMAL) [Procesando].
     Contenido recuperado de trabajo_01.txt:
       Contrato version uno
```

El archivo se escribió una sola vez, al enviar, así que conserva el texto
de entonces. Con `10` se finaliza el trabajo y con `12` se ve el historial.

## Caso del simulador

Con los datos del enunciado, registrados en orden de llegada:

| Trabajo | Páginas | Prioridad |
|---|---|---|
| Documento A | 5 | Normal (2) |
| Documento B | 2 | Alta (1) |
| Documento C | 10 | Normal (2) |
| Documento D | 3 | Alta (1) |
| Documento E | 8 | Baja (3) |

La opción 11 muestra las colas antes de atender nada. Alternando `9` y
`10` cinco veces, el orden de salida es **B, D, A, C, E**.

## Pruebas

```bash
make correr-pruebas    # 42 verificaciones del gestor de impresión
make pruebas-editor    # 22 verificaciones del editor y la pila
make auditoria         # cuenta reservas y liberaciones de memoria
```

Las dos baterías devuelven código de salida 0 solo si todas las
verificaciones pasan, de modo que sirven como prueba de regresión.

`make auditoria` reconstruye las baterías con un contador y reporta
cuántas reservas quedaron vivas al terminar.

## Si `make` falla al enlazar

El GCC que trae Dev-C++ es de 2004 y su enlazador falla en Windows 11.
Hay que instalar un MinGW moderno:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
```

desde MSYS2, y añadir `C:\msys64\ucrt64\bin` al PATH.
