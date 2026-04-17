#include "mapa.h"

// Definir la matriz del mapa
Tile mapa[MAPA_ANCHO][MAPA_ALTO];

// Definir texturas globales
Texture2D arbol1;
Texture2D arbol2;
Texture2D arbusto;
Texture2D desnivel;
Texture2D piedra;
Texture2D piedraPequeña;

void InicializarMapa(void) {
    arbol1 = LoadTexture("Tree2.png");
    arbol2 = LoadTexture("Christmas_tree2.png");
    arbusto = LoadTexture("Fruit_tree3.png");
    desnivel = LoadTexture("Broken_tree3.png");
    piedra = LoadTexture("piedra.png");
    piedraPequeña = LoadTexture("piedrapequeña.png");

    for (int x = 0; x < MAPA_ANCHO; x++) {
        for (int y = 0; y < MAPA_ALTO; y++) {
            mapa[x][y].cespedColor = (Color){ 0, 100, 0, 255 };
            mapa[x][y].arbol = arbol1;
            mapa[x][y].arbusto = arbusto;
            mapa[x][y].tipoTerreno = desnivel;
            mapa[x][y].piedra = piedra;
            mapa[x][y].piedraPequeña = piedraPequeña;
            mapa[x][y].posicion = (Vector2){ x * TILE_SIZE, y * TILE_SIZE };
            mapa[x][y].tieneArbol = false;
            mapa[x][y].tieneArbusto = false;
            mapa[x][y].tieneDesnivel = false;
            mapa[x][y].tienePiedra = false;
            mapa[x][y].tienePiedraPequeña = false;
        }
    }

    for (int i = 0; i < 30; i++) {
        int x = GetRandomValue(0, MAPA_ANCHO - 1);
        int y = GetRandomValue(0, MAPA_ALTO - 1);
        mapa[x][y].tieneArbol = true;
        mapa[x][y].arbol = (GetRandomValue(0, 1) == 0) ? arbol1 : arbol2;
    }

    for (int i = 0; i < 20; i++) {
        int x = GetRandomValue(0, MAPA_ANCHO - 1);
        int y = GetRandomValue(0, MAPA_ALTO - 1);
        mapa[x][y].tieneArbusto = true;
    }

    for (int i = 0; i < 10; i++) {
        int x = GetRandomValue(0, MAPA_ANCHO - 1);
        int y = GetRandomValue(0, MAPA_ALTO - 1);
        mapa[x][y].tieneDesnivel = true;
    }

    for (int i = 0; i < 15; i++) {
        int x = GetRandomValue(0, MAPA_ANCHO - 1);
        int y = GetRandomValue(0, MAPA_ALTO - 1);
        mapa[x][y].tienePiedra = true;
    }

    for (int i = 0; i < 15; i++) {
        int x = GetRandomValue(0, MAPA_ANCHO - 1);
        int y = GetRandomValue(0, MAPA_ALTO - 1);
        mapa[x][y].tienePiedraPequeña = true;
    }
}

void DibujarMapa(void) {
    for (int x = 0; x < MAPA_ANCHO; x++) {
        for (int y = 0; y < MAPA_ALTO; y++) {
            DrawRectangle(mapa[x][y].posicion.x, mapa[x][y].posicion.y, TILE_SIZE, TILE_SIZE, mapa[x][y].cespedColor);

            if (mapa[x][y].tieneDesnivel) {
                DrawTexture(mapa[x][y].tipoTerreno, mapa[x][y].posicion.x, mapa[x][y].posicion.y, WHITE);
            }

            if (mapa[x][y].tieneArbusto) {
                DrawTexture(mapa[x][y].arbusto, mapa[x][y].posicion.x, mapa[x][y].posicion.y, WHITE);
            }

            //if (mapa[x][y].tieneArbol) {
            //    DrawTexture(mapa[x][y].arbol, mapa[x][y].posicion.x, mapa[x][y].posicion.y, WHITE);
            //}

            if (mapa[x][y].tienePiedra) {
                DrawTexture(mapa[x][y].piedra, mapa[x][y].posicion.x, mapa[x][y].posicion.y, WHITE);
            }

            //if (mapa[x][y].tienePiedraPequeña) {
            //    DrawTexture(mapa[x][y].piedraPequeña, mapa[x][y].posicion.x, mapa[x][y].posicion.y, WHITE);
            //}
        }
    }
}
