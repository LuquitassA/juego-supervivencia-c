#include "animales.h"


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define DIRECTION_CHANGE_INTERVAL 2.0f

void InicializarAnimales(Animales* animales) {
    animales->posicion = (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    animales->velocidad = 100.0f;
    animales->texturaAnimal = LoadTexture("vaca.png");
    animales->vida = 25.0f;
    animales->direccion = (Vector2){ 1.0f, 0.0f };  // Dirección inicial
    animales->tiempoDesdeCambio = 0.0f;  // Tiempo desde el último cambio de dirección
    animales->escala = 0.12f;
}

void ActualizarAnimales(Animales* animales, float deltaTime)
{
    // Actualiza el tiempo desde el último cambio de dirección
    animales->tiempoDesdeCambio += deltaTime;

    // Cambia la dirección aleatoriamente si ha pasado el intervalo
    if (animales->tiempoDesdeCambio >= DIRECTION_CHANGE_INTERVAL) {
        // Obtener valores aleatorios y convertirlos a float
        float x = (float)GetRandomValue(-1, 1);
        float y = (float)GetRandomValue(-1, 1);

        // Evita la dirección (0,0)
        if (x == 0 && y == 0) {
            x = 1.0f;
        }

        animales->direccion = (Vector2){ x, y };
        animales->tiempoDesdeCambio = 0.0f;
    }

    // Normaliza la dirección para que el zombi no se mueva más rápido en diagonal
    if (animales->direccion.x != 0 || animales->direccion.y != 0) {
        Vector2 direccionNormalizada = animales->direccion;

        // Actualiza la posición del enemigo
        animales->posicion.x += direccionNormalizada.x * animales->velocidad * deltaTime;
        animales->posicion.y += direccionNormalizada.y * animales->velocidad * deltaTime;
    }

    // Asegúrate de que el enemigo no se salga de la pantalla
    if (animales->posicion.x < 0) animales->posicion.x = 0;
    if (animales->posicion.x > SCREEN_WIDTH) animales->posicion.x = SCREEN_WIDTH;
    if (animales->posicion.y < 0) animales->posicion.y = 0;
    if (animales->posicion.y > SCREEN_HEIGHT) animales->posicion.y = SCREEN_HEIGHT;

}

void muerteAnimales(Animales* animales) {
    if (animales->vida <= 0) {
        animales->vida = 25;
        animales->posicion = (Vector2){ GetRandomValue(-100, 1920), GetRandomValue(-100, 1080) };
    }
}

