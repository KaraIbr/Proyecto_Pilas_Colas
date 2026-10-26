# Pseudocódigo y Diagramas de Memoria — Cola (FIFO)

**Hito 1 · Parte de Aarón** — Módulo del Gestor de Impresión.
La parte de la Pila (TOPE) es de Karina y está en [`pseudocodigo.md`](pseudocodigo.md).

## 1. Qué es FIFO

El primer trabajo que entra es el primero que sale. Hacen falta dos punteros,
porque se entra por un lado y se sale por el otro:

- **FRENTE**: por donde salen los trabajos.
- **FINAL**: por donde entran los trabajos.

La pila usa un solo puntero (TOPE) porque entra y sale por el mismo lado.
Esa es la diferencia entre las dos estructuras.

## 2. Diagramas de memoria

Cola vacía:

```
   frente ---> NULO
   final  ---> NULO
   cantidad: 0
```

Cola con tres trabajos:

```
   frente ---+
   final  ---|---------------------------+
             v                           v
         +-------+    +-------+    +-------+
         | #101  | -> | #102  | -> | #103  | -> NULO
         +-------+    +-------+    +-------+
             ^                         ^
          sale aquí                entra aquí
```

Qué guarda cada nodo:

```
   id, nombre, paginas, prioridad
   texto[1024]   <-- arreglo, no puntero
   siguiente ---> nodo siguiente
```

`texto` es un arreglo dentro del nodo, así que al encolar se **copia** el
documento. Si fuera un puntero, editar el texto después de enviarlo cambiaría
lo que se imprime (caso de prueba 8).

## 3. Pseudocódigo de las primitivas

```
Procedimiento InicializarCola(C)
    C.frente <- NULO
    C.final  <- NULO
    C.cantidad <- 0
Fin

Función ColaVacia(C)
    Devolver (C.frente = NULO)
Fin

Procedimiento Encolar(C, T)        // entra por el FINAL
    nuevo <- ReservarMemoria()
    nuevo.trabajo <- copia de T
    nuevo.siguiente <- NULO
    Si ColaVacia(C) Entonces
        C.frente <- nuevo
        C.final  <- nuevo
    Si no
        C.final.siguiente <- nuevo
        C.final <- nuevo
    Fin Si
    C.cantidad <- C.cantidad + 1
Fin

Función Desencolar(C)              // sale por el FRENTE
    Si ColaVacia(C) Entonces
        Devolver "cola vacía"
    Fin Si
    salida <- C.frente.trabajo
    retirado <- C.frente
    C.frente <- C.frente.siguiente
    Si C.frente = NULO Entonces
        C.final <- NULO            // salió el último
    Fin Si
    LiberarMemoria(retirado)
    C.cantidad <- C.cantidad - 1
    Devolver salida
Fin
```

## 4. Las tres colas de prioridad

```
   ALTA   (1):  frente -> [ ] -> [ ] -> NULO
   NORMAL (2):  frente -> [ ] -> NULO
   BAJA   (3):  frente -> NULO
```

```
Procedimiento Despachar(G)
    Si G.ocupada Entonces
        Devolver "ocupada"         // no expropiativo
    Fin Si
    Si no ColaVacia(G.alta)        Entonces T <- Desencolar(G.alta)
    Si no, si no ColaVacia(G.normal) Entonces T <- Desencolar(G.normal)
    Si no, si no ColaVacia(G.baja)   Entonces T <- Desencolar(G.baja)
    Si no Devolver "no hay trabajos"
    G.en_curso <- T
    G.ocupada <- Verdadero
Fin
```

Tres colas separadas en lugar de una lista ordenada: encolar es directo al
final y el orden de llegada no se puede alterar.

## 5. Traza del caso del simulador

Llegan A(Normal), B(Alta), C(Normal), D(Alta), E(Baja):

```
   ALTA:   B -> D
   NORMAL: A -> C
   BAJA:   E
```

Se vacía Alta, luego Normal, luego Baja: **B, D, A, C, E**.

## 6. Correspondencia con el código

| Pseudocódigo | Función en C |
|---|---|
| InicializarCola | `cola_inicializar` |
| ColaVacia | `cola_esta_vacia` |
| Encolar | `cola_encolar` |
| Desencolar | `cola_desencolar` |
| Despachar | `gestor_iniciar_siguiente` |
