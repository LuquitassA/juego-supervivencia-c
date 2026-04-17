#ifndef JUGADOR_H
#define JUGADOR_H

#include "raylib.h"
#include "enemigo.h"
#include "animales.h"
#include "recursos.h"



#define MAX_INVENTORY_SLOTS 10

int Selec;

typedef enum {
    ITEM_NONE,
    ITEM_WEAPON,
    ITEM_HEALTH_POTION,
    ITEM_ARMOR,
    ITEM_WOOD,     
    ITEM_STONE
} ItemType;

typedef struct Item{
    ItemType tipo;
    int cantidad;
    Texture2D icono;
    char nombre[50];
    int valor;  // Podría ser daño para armas, puntos de curación para pociones, etc.
} Item;

typedef struct Jugador {
    Vector2 posicion;
    float velocidad;
    Texture2D textura;
    float vida;
    int daño;
    float tiempoUltimoAtaque;
    float tiempoEnfriamientoAtaque;
    struct Item inventario[MAX_INVENTORY_SLOTS];
    int itemsInventario;  // Número actual de items en el inventario
} Jugador;

void InicializarInventario(Jugador* jugador);
bool AgregarItemAlInventario(Jugador* jugador, Item nuevoItem);
bool RemoverItemDelInventario(Jugador* jugador, int indice);
void UsarItem(Jugador* jugador, int indice);
void DibujarInventario(Jugador* jugador);
void interaccionInventario(Jugador* jugador);
bool VerificarColision(Vector2 posicionJugador);

void InicializarJugador(Jugador* jugador);
void MoverJugador(Jugador* jugador);
bool DetectarColisionCircular(Vector2 pos1, Vector2 pos2, float radio);
void AplicarDaño(Jugador* jugador, Animales* animales, int numAnimales, Enemigo* enemigos, int numEnemigos);
void DibujarAreaAtaque(Jugador jugador);

#endif // JUGADOR_H

