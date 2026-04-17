#ifndef REFUGIO_H
#define REFUGIO_H

#include "raylib.h"
#include "jugador.h"

#define MAX_REFUGIOS 10

typedef struct {
    Vector2 posicion;
    bool activo;
    int vida;  // Durabilidad del refugio
    Texture2D textura;
} Refugio;

typedef struct {
    Refugio refugios[MAX_REFUGIOS];
    int cantidadRefugios;
} Refugios;

void InicializarRefugios(Refugios* refugios);
bool CrearRefugio(Refugios* refugios, Jugador* jugador);
void DibujarRefugios(Refugios* refugios);

#endif // REFUGIO_H
