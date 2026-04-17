#ifndef RECURSOS_H
#define RECURSOS_H
#include "raylib.h"
#include "jugador.h"

#define MAX_RECURSOS 50

typedef enum {
    RECURSO_ARBOL,
    RECURSO_PIEDRA
} TipoRecurso;

typedef struct {
    Vector2 posicion;
    TipoRecurso tipo;
    Texture2D textura;
    bool activo;
    int vida;  // Golpes necesarios para recolectar
} Recurso;

typedef struct {
    Recurso recursos[MAX_RECURSOS];
    int cantidadRecursos;
} Recursos;

void InicializarRecursos(Recursos* recursos);
void DibujarRecursos(Recursos* recursos);

//bool RecolectarRecurso(Recursos* recursos, Jugador* jugador);

#endif // RECURSOS_H