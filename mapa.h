#ifndef MAPA_H
#define MAPA_H

#include "raylib.h"

#define MAPA_ANCHO 100
#define MAPA_ALTO 100
#define TILE_SIZE 64

typedef struct Tile {
    Color cespedColor;
    Texture2D arbol;
    Texture2D arbusto;
    Texture2D tipoTerreno;
    Texture2D piedra;
    Texture2D piedraPequeña;
    Vector2 posicion;
    bool tieneArbol;
    bool tieneArbusto;
    bool tieneDesnivel;
    bool tienePiedra;
    bool tienePiedraPequeña;
} Tile;

extern Tile mapa[MAPA_ANCHO][MAPA_ALTO]; // Declarar la matriz del mapa

void InicializarMapa(void);
void DibujarMapa(void);

#endif // MAPA_H
