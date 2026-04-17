#include "recursos.h"
#include <stdlib.h>
#include "math.h"


void InicializarRecursos(Recursos* recursos) {
    recursos->cantidadRecursos = 0;

    // Cargar texturas
    Texture2D texturaArbol = LoadTexture("Tree2.png");
    Texture2D texturaPiedra = LoadTexture("piedrapequeña.png");

    // Generar recursos aleatorios
    for (int i = 0; i < 1000; i++) {
        Recurso nuevoRecurso;
        nuevoRecurso.posicion = (Vector2){
            rand() % 2000,
            rand() % 2000
        };
        nuevoRecurso.activo = true;

        if (rand() % 2 == 0) {
            nuevoRecurso.tipo = RECURSO_ARBOL;
            nuevoRecurso.textura = texturaArbol;
            nuevoRecurso.vida = 3;  // Golpes para talar
        }
        else {
            nuevoRecurso.tipo = RECURSO_PIEDRA;
            nuevoRecurso.textura = texturaPiedra;
            nuevoRecurso.vida = 2;  // Golpes para romper
        }
        if (recursos->cantidadRecursos < MAX_RECURSOS) {
            recursos->recursos[recursos->cantidadRecursos++] = nuevoRecurso;
        }
        else {
            break; // Evitar exceder el límite
        }
    }
}

void DibujarRecursos(Recursos* recursos) {
    for (int i = 0; i < recursos->cantidadRecursos; i++) {
        if (recursos->recursos[i].activo) {
            DrawTextureEx(
                recursos->recursos[i].textura,
                recursos->recursos[i].posicion,
                0.0f, 1.0F, WHITE
            );
        }
    }
}

bool RecolectarRecurso(Recursos* recursos, Jugador* jugador) {
    for (int i = 0; i < recursos->cantidadRecursos; i++) {
        if (!recursos->recursos[i].activo) continue;

        float dx = jugador->posicion.x - recursos->recursos[i].posicion.x;
        float dy = jugador->posicion.y - recursos->recursos[i].posicion.y;
        float distancia = sqrt(dx * dx + dy * dy);

        // Distancia más corta para recolección
        if (distancia < 50) {
            recursos->recursos[i].vida =- 1;

            if (recursos->recursos[i].vida <= 0) {
                Item nuevoItem;
                nuevoItem.cantidad = 2;

                if (recursos->recursos[i].tipo == RECURSO_ARBOL) {
                    nuevoItem.tipo = ITEM_WOOD;
                    strcpy(nuevoItem.nombre, "Tronco");
                }
                else {
                    nuevoItem.tipo = ITEM_STONE;
                    strcpy(nuevoItem.nombre, "Piedra");
                }

                nuevoItem.icono = LoadTexture(
                    nuevoItem.tipo == ITEM_WOOD ?
                    "tronco.png" : "piedra.png"
                );
                nuevoItem.valor = 0;

                AgregarItemAlInventario(jugador, nuevoItem);
                recursos->recursos[i].activo = false;
                return true;
            }
        }
    }
    return false;
}