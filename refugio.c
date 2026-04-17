#include "refugio.h"
#include <stdlib.h>
#include <math.h>

void InicializarRefugios(Refugios* refugios) {
    refugios->cantidadRefugios = 0;
}

bool CrearRefugio(Refugios* refugios, Jugador* jugador) {
    // Verificar si hay espacio para más refugios
    if (refugios->cantidadRefugios >= MAX_REFUGIOS) {
        return false;
    }

    // Verificar si el jugador tiene suficientes recursos
    int madera = 0, piedras = 0;
    for (int i = 0; i < MAX_INVENTORY_SLOTS; i++) {
        if (jugador->inventario[i].tipo == ITEM_WOOD) {
            madera += jugador->inventario[i].cantidad;
        }
        if (jugador->inventario[i].tipo == ITEM_STONE) {
            piedras += jugador->inventario[i].cantidad;
        }
    }

    // Requiere 5 maderas y 3 piedras para crear un refugio
    if (madera >= 5 && piedras >= 3) {
        // Consumir recursos
        int maderaConsumida = 0, piedrasConsumidas = 0;
        for (int i = 0; i < MAX_INVENTORY_SLOTS && (maderaConsumida < 5 || piedrasConsumidas < 3); i++) {
            if (jugador->inventario[i].tipo == ITEM_WOOD && maderaConsumida < 5) {
                int consumir = fminf(jugador->inventario[i].cantidad, 5 - maderaConsumida);
                jugador->inventario[i].cantidad -= consumir;
                maderaConsumida += consumir;
                if (jugador->inventario[i].cantidad <= 0) {
                    RemoverItemDelInventario(jugador, i);
                }
            }
            if (jugador->inventario[i].tipo == ITEM_STONE && piedrasConsumidas < 3) {
                int consumir = fminf(jugador->inventario[i].cantidad, 3 - piedrasConsumidas);
                jugador->inventario[i].cantidad -= consumir;
                piedrasConsumidas += consumir;
                if (jugador->inventario[i].cantidad <= 0) {
                    RemoverItemDelInventario(jugador, i);
                }
            }
        }

        // Crear refugio
        Refugio nuevoRefugio;
        nuevoRefugio.posicion = jugador->posicion;
        nuevoRefugio.activo = true;
        nuevoRefugio.vida = 100;  // Durabilidad inicial
        nuevoRefugio.textura = LoadTexture("refugio.png");  // Asegúrate de tener esta textura

        refugios->refugios[refugios->cantidadRefugios++] = nuevoRefugio;
        return true;
    }

    return false;
}

void DibujarRefugios(Refugios* refugios) {
    for (int i = 0; i < refugios->cantidadRefugios; i++) {
        if (refugios->refugios[i].activo) {
            DrawTextureEx(
                refugios->refugios[i].textura,
                refugios->refugios[i].posicion,
                0.0f, 0.07f, WHITE
            );
        }
    }
}