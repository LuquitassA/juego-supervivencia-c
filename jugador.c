#include "jugador.h"
#include "mapa.h"  // Para poder usar el mapa y las funciones de colisión
#include "raylib.h"
#include <math.h>


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Función para inicializar al jugador
void InicializarJugador(Jugador* jugador) {
    jugador->posicion = (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    jugador->velocidad = 200.0f;
    jugador->textura = LoadTexture("Jugador.png");
    jugador->vida = 100.0f;
    jugador->daño = 10;
}

// Función para verificar si hay colisión con objetos (árboles y arbustos)
bool VerificarColision(Vector2 posicionJugador) {
    int tileX = (int)(posicionJugador.x / TILE_SIZE);
    int tileY = (int)(posicionJugador.y / TILE_SIZE);

    // Verificar límites del mapa
    if (tileX < 0 || tileY < 0 || tileX >= MAPA_ANCHO || tileY >= MAPA_ALTO) {
        return true; // Considerar fuera de límites como colisión
    }

    // Verificar si el tile tiene un árbol, arbusto o piedra
    if (mapa[tileX][tileY].tieneArbol || mapa[tileX][tileY].tieneArbusto || mapa[tileX][tileY].tienePiedra) {
        return true; // Hay colisión
    }
    return false; // No hay colisión
}

// Función para mover al jugador
void MoverJugador(Jugador* jugador) {
    // Dirección de movimiento
    Vector2 direccion = { 0, 0 };

    // Movimiento rápido con Shift
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        if (IsKeyDown(KEY_D)) direccion.x += 2;
        if (IsKeyDown(KEY_A)) direccion.x -= 2;
        if (IsKeyDown(KEY_W)) direccion.y -= 2;
        if (IsKeyDown(KEY_S)) direccion.y += 2;
    }
    // Movimiento más lento con Control izquierdo
    else if (IsKeyDown(KEY_LEFT_CONTROL)) {
        if (IsKeyDown(KEY_D)) direccion.x += 0.5;
        if (IsKeyDown(KEY_A)) direccion.x -= 0.5;
        if (IsKeyDown(KEY_W)) direccion.y -= 0.5;
        if (IsKeyDown(KEY_S)) direccion.y += 0.5;
    }
    else {
        if (IsKeyDown(KEY_D)) direccion.x += 1;
        if (IsKeyDown(KEY_A)) direccion.x -= 1;
        if (IsKeyDown(KEY_W)) direccion.y -= 1;
        if (IsKeyDown(KEY_S)) direccion.y += 1;
    }

    // Nueva posición proyectada
    Vector2 nuevaPosicion = { jugador->posicion.x + direccion.x * jugador->velocidad * GetFrameTime(),
                              jugador->posicion.y + direccion.y * jugador->velocidad * GetFrameTime() };

    // Verificar si la nueva posición colisiona con objetos
    if (!VerificarColision(nuevaPosicion)) {
        jugador->posicion = nuevaPosicion; // Si no hay colisión, actualiza la posición
    }

    // Limitar el movimiento del jugador al mapa
    if (jugador->posicion.x < 0) jugador->posicion.x = 0;
    if (jugador->posicion.y < 0) jugador->posicion.y = 0;
    if (jugador->posicion.x > (MAPA_ANCHO * TILE_SIZE) - TILE_SIZE) jugador->posicion.x = (MAPA_ANCHO * TILE_SIZE) - TILE_SIZE;
    if (jugador->posicion.y > (MAPA_ALTO * TILE_SIZE) - TILE_SIZE) jugador->posicion.y = (MAPA_ALTO * TILE_SIZE) - TILE_SIZE;
}
bool DetectarColisionCircular(Vector2 pos1, Vector2 pos2, float radio) {
    float dx = pos1.x - pos2.x;
    float dy = pos1.y - pos2.y;
    float distancia = sqrt(dx * dx + dy * dy);
    return distancia <= radio;
}

// En jugador.c
void AplicarDaño(Jugador* jugador, Animales* animales, int numAnimales, Enemigo* enemigos, int numEnemigos) {
    float tiempoActual = GetTime();
    float radioAtaque = 50.0f;  // Distancia máxima de ataque
    float anguloAtaque = 90.0f; // Ángulo de ataque en grados

    if (IsKeyPressed(KEY_SPACE) &&
        (tiempoActual - jugador->tiempoUltimoAtaque) >= jugador->tiempoEnfriamientoAtaque) {

        jugador->tiempoUltimoAtaque = tiempoActual;

        // Verificar daño a animales
        for (int i = 0; i < numAnimales; i++) {
            if (animales[i].vida > 0) {
                // Calcular la distancia entre el jugador y el animal
                float dx = jugador->posicion.x - animales[i].posicion.x;
                float dy = jugador->posicion.y - animales[i].posicion.y;
                float distancia = sqrtf(dx * dx + dy * dy);

                // Solo aplicar daño si está dentro del radio de ataque
                if (distancia <= radioAtaque) {
                    animales[i].vida -= jugador->daño;
                    if (animales[i].vida <= 0) {
                        animales[i].vida = 0;
                        Item carne = {
                            .tipo = ITEM_HEALTH_POTION,
                            .cantidad = 3,
                            .valor = 20.0f,
                            .nombre = "Carne Cruda"
                        };
                        carne.icono = LoadTexture("carnecruda.png");  // Reemplaza con tu ruta de imagen
                        AgregarItemAlInventario(jugador, carne);
                    }
                }
            }
        }

        // Verificar daño a enemigos
        for (int i = 0; i < numEnemigos; i++) {
            if (enemigos[i].vida > 0) {
                // Calcular la distancia entre el jugador y el enemigo
                float dx = jugador->posicion.x - enemigos[i].posicion.x;
                float dy = jugador->posicion.y - enemigos[i].posicion.y;
                float distancia = sqrtf(dx * dx + dy * dy);

                // Solo aplicar daño si está dentro del radio de ataque
                if (distancia <= radioAtaque) {
                    enemigos[i].vida -= jugador->daño;
                    if (enemigos[i].vida <= 0) {
                        enemigos[i].vida = 0;
                    }
                }
            }
        }
    }
}


// Función para visualizar el área de ataque
void DibujarAreaAtaque(Jugador jugador) {
    float radioAtaque = 50.0f;
    if (IsKeyDown(KEY_SPACE)) {
        // Dibuja un círculo semitransparente para mostrar el área de ataque
        DrawCircle(jugador.posicion.x, jugador.posicion.y,radioAtaque,(Color) {255, 0, 0, 100});
        // Dibuja el borde del círculo
        DrawCircleLines(jugador.posicion.x,jugador.posicion.y,radioAtaque,RED);
    }
}


//inventario
void InicializarInventario(Jugador* jugador) {
    jugador->itemsInventario = 0;
    for (int i = 0; i < MAX_INVENTORY_SLOTS; i++) {
        jugador->inventario[i].tipo = ITEM_NONE;
        jugador->inventario[i].cantidad = 0;
    }
}

bool AgregarItemAlInventario(Jugador* jugador, Item nuevoItem) {
    // Primero, intenta apilar items del mismo tipo
    for (int i = 0; i < MAX_INVENTORY_SLOTS; i++) {
        if (jugador->inventario[i].tipo == nuevoItem.tipo) {
            jugador->inventario[i].cantidad += nuevoItem.cantidad;
            return true;
        }
    }

    // Si no se pudo apilar, busca un slot vacío
    for (int i = 0; i < MAX_INVENTORY_SLOTS; i++) {
        if (jugador->inventario[i].tipo == ITEM_NONE) {
            jugador->inventario[i] = nuevoItem;
            jugador->itemsInventario++;
            return true;
        }
    }

    return false;  // Inventario lleno
}

bool RemoverItemDelInventario(Jugador* jugador, int indice) {
    if (indice < 0 || indice >= MAX_INVENTORY_SLOTS) return false;

    if (jugador->inventario[indice].tipo != ITEM_NONE) {
        jugador->inventario[indice].tipo = ITEM_NONE;
        jugador->inventario[indice].cantidad = 0;
        jugador->itemsInventario--;
        return true;
    }

    return false;
}

void UsarItem(Jugador* jugador, int indice) {
    if (indice < 0 || indice >= MAX_INVENTORY_SLOTS) return;

    Item* item = &jugador->inventario[indice];

    switch (item->tipo) {
    case ITEM_HEALTH_POTION:
        // Usar poción de salud
        jugador->vida = fminf(100, jugador->vida + item->valor);
        item->cantidad--;

        if (item->cantidad <= 0) {
            RemoverItemDelInventario(jugador, indice);
        }
        break;

    case ITEM_WEAPON:
        // Cambiar/equipar arma (podría implementarse más adelante)
        jugador->daño = item->valor;
        break;

    case ITEM_ARMOR:
        // Aumentar defensa (podría implementarse más adelante)
        break;

    default:
        break;
    }
}

void DibujarInventario(Jugador* jugador) {
    // Dibujar recuadro de inventario
    DrawRectangle(10, 10, 200, 50 * MAX_INVENTORY_SLOTS, DARKGRAY);

    for (int i = 0; i < MAX_INVENTORY_SLOTS; i++) {
        if (jugador->inventario[i].tipo != ITEM_NONE) {
            // Dibujar icono de item
            DrawTextureEx(jugador->inventario[i].icono,
                (Vector2) {
                20, 20 + i * 50
            },
                0.0f, 0.07f, WHITE);

            // Dibujar cantidad
            char cantidadTexto[10];
            sprintf(cantidadTexto, "x%d", jugador->inventario[i].cantidad);
            DrawText(cantidadTexto, 140, 40 + i * 50, 20, WHITE);
        }
    }
}

void interaccionInventario(Jugador* jugador){
    if (IsKeyPressed(KEY_ONE)) {        
        if (jugador->itemsInventario > 0) {
         UsarItem(jugador, 0);
        }
    }
    if (IsKeyPressed(KEY_TWO)) {
        if (jugador->itemsInventario > 0) {
            UsarItem(jugador, 1);
        }
    }
    if (IsKeyPressed(KEY_THREE)) {
        if (jugador->itemsInventario > 0) {
            UsarItem(jugador, 2);
        }
    }
    if (IsKeyPressed(KEY_FOUR)) {
        if (jugador->itemsInventario > 0) {
            UsarItem(jugador, 3);
        }
    }
    if (IsKeyPressed(KEY_FIVE)) {
        if (jugador->itemsInventario > 0) {
            UsarItem(jugador, 4);
        }
    }
    if (IsKeyPressed(KEY_SIX)) {
        if (jugador->itemsInventario > 0) {
            UsarItem(jugador, 0);
        }
    }
    if (IsKeyPressed(KEY_SEVEN)) {
        if (jugador->itemsInventario > 0) {
            UsarItem(jugador, 0);
        }
    }
    if (IsKeyPressed(KEY_EIGHT)) {
        if (jugador->itemsInventario > 0) {
            UsarItem(jugador, 0);
        }
    }
    if (IsKeyPressed(KEY_NINE)) {
        if (jugador->itemsInventario > 0) {
            UsarItem(jugador, 0);
        }
    }
    if (IsKeyPressed(KEY_ZERO)) {
        if (jugador->itemsInventario > 0) {
            UsarItem(jugador, 0);
        }
    }
}
