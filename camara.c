#include "camara.h"
#include "mapa.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void InicializarCamara(Camera2D* camara) {
    camara->offset = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    camara->rotation = 0.0f;
    camara->zoom = 1.0f;
    camara->target = (Vector2){ 0.0f, 0.0f };
}

void ActualizarCamara(Camera2D* camara, Jugador* jugador) {
    camara->target = jugador->posicion;

    // Definir límites para la cámara
    float limiteDerecho = MAPA_ANCHO * TILE_SIZE - SCREEN_WIDTH;
    float limiteInferior = MAPA_ALTO * TILE_SIZE - SCREEN_HEIGHT;

    // Limitar la cámara dentro del área del mapa
    if (camara->target.x < SCREEN_WIDTH / 2.0f) camara->target.x = SCREEN_WIDTH / 2.0f;
    if (camara->target.y < SCREEN_HEIGHT / 2.0f) camara->target.y = SCREEN_HEIGHT / 2.0f;
    if (camara->target.x > limiteDerecho + SCREEN_WIDTH / 2.0f) camara->target.x = limiteDerecho + SCREEN_WIDTH / 2.0f;
    if (camara->target.y > limiteInferior + SCREEN_HEIGHT / 2.0f) camara->target.y = limiteInferior + SCREEN_HEIGHT / 2.0f;
}
