# PLAN DE TRABAJO Y GESTIÓN DE REPOSITORIO
**Proyecto Integrador · Unidad 4: Pilas y Colas**

**Asignatura:** Estructuras de Datos y Compiladores

**Docente:** Dra. Blanca Guadalupe Estrada Rentería

**Institución:** Global University

**Programa:** Ing. en Seguridad Tecnológica y Desarrollo de Software

**Integrantes:** Karina Patricia Ibarra Morales · Aarón Salvador Castañeda Ruiz

**Fecha de Entrega y Presentación:** Lunes 26 de octubre de 2026

## Compilación y ejecución

**Requisito:** GCC con soporte C99 (MinGW-w64 en Windows) y `make`.

```bash
make                 # compila la aplicación en bin/editor_impresion
make correr-pruebas  # ejecuta las 42 verificaciones del gestor de impresión
make pruebas-editor  # ejecuta las 22 verificaciones del editor y la pila
make auditoria       # cuenta reservas y liberaciones de memoria
make limpiar         # borra los archivos generados
```

Para ejecutar la aplicación:

```bash
./bin/editor_impresion
```

El menú agrupa las operaciones del editor (1 a 7), las del gestor de impresión (8 a 12) y el guardado en archivo (13). Las dos baterías de pruebas devuelven código de salida 0 solo si todas las verificaciones pasan, de modo que sirven como prueba de regresión.

Al enviar un documento a impresión se escribe un archivo `trabajo_NN.txt` en el directorio desde el que se lanzó el programa. Ese archivo es el contenido que se imprime.

Los pasos detallados, el menú completo y un flujo de ejemplo están en [`docs/guia-ejecucion.md`](docs/guia-ejecucion.md).

## 1. Configuración de Entorno, Licenciamiento y Git

### 1.1. Licencia del Proyecto

- **Tipo:** **MIT License**
    
- **Justificación:** Estándar académico de código abierto y permisivo que permite la libre inspección, compilación y modificación del código fuente con fines educativos y de evaluación.


### 1.2. Política de Ramas (Git Flow Simplificado)

- **`main`**: Rama de producción protegida. Contiene únicamente versiones terminadas, estables y probadas para la entrega oficial.
    
- **`develop`**: Rama principal de integración y desarrollo activo.
    
- **`feature/<nombre-tarea>`**: Ramas temporales de trabajo individual. Se desprenden de `develop` y se reintegran mediante _pull request_ / _merge_ al cumplir su objetivo.
    

### 1.3. Estructura del Repositorio

Plaintext


proyecto_unidad4/
├── LICENSE              # Licencia MIT
├── README.md            # Instrucciones de compilación y ejecución
├── Makefile             # Reglas de compilación automatizada con GCC (-Wall)
├── include/             # Archivos de cabecera (.h)
│   ├── pila.h
│   ├── cola.h
│   ├── editor.h
│   ├── impresion.h
│   └── ui.h
├── src/                 # Código fuente en C (.c)
│   ├── pila.c
│   ├── cola.c
│   ├── editor.c
│   ├── impresion.c
│   ├── ui.c
│   └── main.c
└── docs/                # Pseudocódigo, diagramas y evidencias para el reporte PDF


## 2. Cronograma de Trabajo por Hitos y Commits

### HITO 1: Estructura Base, Licenciamiento y Pseudocódigo

- **Ventana de tiempo:** Día 1 (Fase Matutina)
    
- **Objetivo:** Comprender la mecánica de memoria antes de codificar, definir interfaces y levantar la base del proyecto.
    

|**Rama**|**Responsable**|**Mensaje de Commit (Convencional)**|**Descripción de la Tarea**|
|---|---|---|---|
|`main`|Ambos|`chore: initial repository setup and add MIT license`|Crear estructura de directorios, archivo `LICENSE` y `.gitignore`.|
|`develop`|Ambos|`docs: add pseudocode and structural memory diagrams`|Documentar pseudocódigo y diagramas de TOPE (Pila) y Frente/Final (Cola) en `docs/`.|
|`develop`|Ambos|`build: setup Makefile and header interface skeletons`|Crear `Makefile` base y definir las firmas de funciones en `include/pila.h` e `include/cola.h`.|

### HITO 2: Implementación Modular de Estructuras (Lógica en C)

- **Ventana de tiempo:** Día 1 (Fase Vespertina) – Día 2 (Fase Matutina)
    
- **Objetivo:** Implementar la mecánica LIFO y FIFO de forma desacoplada y validar su funcionamiento interno.
    

| **Rama**                   | **Responsable** | **Mensaje de Commit (Convencional)**                                | **Descripción de la Tarea**                                                                                    |
| -------------------------- | --------------- | ------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------- |
| `feature/editor-pila`      | Karina          | `feat(pila): implement stack data structure and LIFO operations`    | Implementar `src/pila.c` (`push`, `pop`, `peek`, manejo explícito de `TOPE`).                                  |
| `feature/editor-pila`      | Karina          | `feat(editor): implement text buffer and undo mechanism`            | Implementar `src/editor.c`, guardado de copia de estado previo a edición y Deshacer.                           |
| `feature/gestor-impresion` | Aarón            | `feat(cola): implement FIFO queue primitives`                       | Implementar `src/cola.c` (`enqueue`, `dequeue`, validaciones de cola vacía).                                   |
| `feature/gestor-impresion` | Aarón            | `feat(impresion): implement 3 priority queues and dispatcher`       | Implementar `src/impresion.c` con colas Alta (1), Normal (2) y Baja (3), y política FIFO dentro de cada nivel. |
| `develop`                  | Ambos           | `merge: integrate feature/editor-pila and feature/gestor-impresion` | Integrar y compilar ambas estructuras en `develop` tras pruebas unitarias básicas.                             |

### HITO 3: Integración del Flujo y Design System en Consola

- **Ventana de tiempo:** Día 2 (Fase Vespertina)
    
- **Objetivo:** Conectar el editor con la cola de impresión y construir la interfaz de terminal basada en cajas y estados.
    

|**Rama**|**Responsable**|**Mensaje de Commit (Convencional)**|**Descripción de la Tarea**|
|---|---|---|---|
|`feature/cli-ui`|Karina|`feat(ui): add clean terminal layout and box drawings`|Implementar `src/ui.c` con bordes Unicode limpios e indicadores visuales (`[TOPE]`, `[ALTA]`).|
|`feature/cli-ui`|Aarón|`feat(main): integrate workflow from editor to print queue`|Conectar en `src/main.c` el paso de copia inmutable del texto al enviarlo a imprimir.|
|`feature/cli-ui`|Ambos|`fix(validation): enforce positive pages and valid priority inputs`|Validar que páginas sea $> 0$ y rechazar prioridades fuera de rango (1, 2, 3).|
|`develop`|Ambos|`merge: feature/cli-ui into develop`|Consolidar la aplicación interactiva completa en `develop`.|

### HITO 4: Batería de Pruebas, Documentación y Congelamiento

- **Ventana de tiempo:** Día 3
    
- **Objetivo:** Ejecutar los 8 casos obligatorios de la rúbrica, redactar el reporte PDF y preparar la defensa.
    

|**Rama**|**Responsable**|**Mensaje de Commit (Convencional)**|**Descripción de la Tarea**|
|---|---|---|---|
|`develop`|Ambos|`test: execute and log 8 mandatory rubric cases`|Correr el caso de prueba del simulador (B $\to$ D $\to$ A $\to$ C $\to$ E) y registrar salidas en consola.|
|`develop`|Karina|`docs: complete technical report sections and architecture diagrams`|Redactar secciones de diseño, explicación teórica LIFO/FIFO y diagramas de flujo.|
|`develop`|Aarón|`docs: complete test evidence and member contribution records`|Incluir capturas de pantalla de los 8 casos y registrar el desglose de aportes individuales.|
|`main`|Ambos|`release: v1.0.0 final project delivery`|Merge fast-forward de `develop` a `main` y etiquetado con `git tag v1.0.0`.|

## 3. Matriz de Cobertura de Pruebas Obligatorias

|**#**|**Caso de Prueba Oficial**|**Módulo**|**Procedimiento de Verificación**|

|**1**|Múltiples trabajos con la misma prioridad|Impresión|Encolar trabajos en el mismo nivel y verificar salida estricta FIFO.|
|**2**|Trabajos con prioridades diferentes|Impresión|Despachar trabajos con niveles mezclados; validar que Alta precede a Normal y Baja.|
|**3**|Llegada de trabajo urgente durante proceso|Impresión|Simular procesamiento activo y verificar que el nuevo trabajo Alta no interrumpe el curso.|
|**4**|Intento de procesar sin trabajos pendientes|Impresión|Solicitar despacho con las tres colas vacías; verificar mensaje informativo sin error.|
|**5**|Múltiples modificaciones consecutivas|Editor|Realizar adiciones y eliminaciones sucesivas verificando que cada estado previo se apila.|
|**6**|Deshacer ejecutado varias veces seguidas|Editor|Ejecutar Deshacer repetidamente comprobando la recuperación inversa LIFO.|
|**7**|Deshacer con pila de historial vacía|Editor|Ejecutar Deshacer sin estados previos guardados; verificar aviso sin fallos en el sistema.|
|**8**|Envío de documento editado a impresión|Integración|Enviar documento, modificar texto en el editor y validar que la copia en cola no cambió.|

## 4. Distribución para la Defensa Oral (20 Puntos)

- **Karina:**
    
    - Exposición de la arquitectura general del sistema.
        
    - Justificación técnica de la Pila (LIFO): demostración de puntero `TOPE`, operaciones `Pone` / `Quita`, y por qué consultar/deshacer no apila nuevos estados.
        
    - Ejecución en vivo de la secuencia de edición y recuperación de texto.
        
- **Aarón:**
    
    - Justificación técnica del Gestor de Impresión: uso de 3 colas FIFO independientes frente a un simple ordenamiento de listas.
        
    - Demostración de la regla no expropiativa (trabajo en curso no se interrumpe).
        
    - Ejecución en vivo del caso del simulador (B $\to$ D $\to$ A $\to$ C $\to$ E) y atención a pruebas con datos aleatorios solicitados por la docente.
