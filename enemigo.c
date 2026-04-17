#include "enemigo.h"
#include "jugador.h"
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define DIRECTION_CHANGE_INTERVAL 2.0f
#define ATAQUE_DISTANCIA 30.0f
#define MAX_DELTA_MOVEMENT 5.0f  // Máximo movimiento por frame

static int numEnemigos = 300;


Vector2 NormalizarVector(Vector2 vector) {
    float length = sqrtf(vector.x * vector.x + vector.y * vector.y);
    if (length > 0) {
        return (Vector2) { vector.x / length, vector.y / length };
    }
    return (Vector2) { 1.0f, 0.0f };
}

void InicializarEnemigos(Enemigo* enemigos, int numEnemigos) {
    for (int i = 0; i < numEnemigos; i++) {
        // Posición inicial con margen para evitar spawns en los bordes
        enemigos[i].posicion.x = GetRandomValue(-1000, 10000);
        enemigos[i].posicion.y = GetRandomValue(-1000, 10000);

        enemigos[i].velocidad = 50.0f;  // Velocidad reducida para mejor control
        enemigos[i].texturaEnemigo = LoadTexture("zombi.png");
        enemigos[i].vida = 50;
        enemigos[i].daño = 1;
        enemigos[i].rangoDeAtaque = 30;
        enemigos[i].tiempoUltimoAtaque = 0.0f;  // Comienza con cooldown
        enemigos[i].cooldownAtaque = 1000.0f;


        // Inicializar dirección normalizada
        enemigos[i].direccion = NormalizarVector((Vector2) {
            (float)GetRandomValue(-100, 100) / 100.0f,
            (float)GetRandomValue(-100, 100) / 100.0f
        });

        enemigos[i].tiempoDesdeCambio = 0.0f;
        enemigos[i].escala = 0.07f;
    }
}

void AplicarDañoAlJugador(Jugador* jugador, Enemigo* enemigos, int numEnemigos, float deltaTime) {
    for (int i = 0; i < numEnemigos; i++) {
        Enemigo* enemigo = &enemigos[i];

        // Incrementamos el tiempo del cooldown del enemigo
        enemigo->tiempoUltimoAtaque += deltaTime;

        // Comprobamos si el enemigo puede atacar
        if (enemigo->tiempoUltimoAtaque >= enemigo->cooldownAtaque) {
            // Calculamos la distancia entre el enemigo y el jugador
            float dx = enemigo->posicion.x - jugador->posicion.x;
            float dy = enemigo->posicion.y - jugador->posicion.y;
            float distancia = sqrt(dx * dx + dy * dy);  // Distancia euclidiana

            // Verificamos si el enemigo está dentro del rango de ataque
            if (distancia <= enemigo->rangoDeAtaque) {
                // El enemigo está en el rango de ataque, inflige daño al jugador
                jugador->vida -= enemigo->daño;  // Daño infligido por el enemigo

                // Reseteamos el cooldown del enemigo
                enemigo->tiempoUltimoAtaque = 0.0f;
            }
        }
    }
}

void ActualizarEnemigos(Enemigo* enemigos, int numEnemigos, float deltaTime) {
    if (!enemigos || deltaTime <= 0) return;

    for (int i = 0; i < numEnemigos; i++) {
        Enemigo* enemigo = &enemigos[i];

        

        // Cambio de dirección gradual
        if (enemigo->tiempoDesdeCambio >= DIRECTION_CHANGE_INTERVAL) {
            // Calcular nueva dirección
            Vector2 nuevaDireccion = NormalizarVector((Vector2) {
                (float)GetRandomValue(-100, 100) / 100.0f,
                (float)GetRandomValue(-100, 100) / 100.0f
            });

            // Hacer la transición más suave
            enemigo->direccion.x = (enemigo->direccion.x + nuevaDireccion.x) * 0.5f;
            enemigo->direccion.y = (enemigo->direccion.y + nuevaDireccion.y) * 0.5f;
            enemigo->direccion = NormalizarVector(enemigo->direccion);

            enemigo->tiempoDesdeCambio = 0.0f;
        }

        // Calcular el movimiento
        float deltaX = enemigo->direccion.x * enemigo->velocidad * deltaTime;
        float deltaY = enemigo->direccion.y * enemigo->velocidad * deltaTime;

        // Limitar el movimiento máximo por frame
        deltaX = fmaxf(fminf(deltaX, MAX_DELTA_MOVEMENT), -MAX_DELTA_MOVEMENT);
        deltaY = fmaxf(fminf(deltaY, MAX_DELTA_MOVEMENT), -MAX_DELTA_MOVEMENT);

        // Calcular nueva posición
        float nuevaX = enemigo->posicion.x + deltaX;
        float nuevaY = enemigo->posicion.y + deltaY;

        // Manejar colisiones con los bordes de la pantalla
        if (nuevaX < 0) {
            nuevaX = 0;
            enemigo->direccion.x *= -1;
        }
        else if (nuevaX > SCREEN_WIDTH) {
            nuevaX = SCREEN_WIDTH;
            enemigo->direccion.x *= -1;
        }

        if (nuevaY < 0) {
            nuevaY = 0;
            enemigo->direccion.y *= -1;
        }
        else if (nuevaY > SCREEN_HEIGHT) {
            nuevaY = SCREEN_HEIGHT;
            enemigo->direccion.y *= -1;
        }

        // Actualizar la posición
        enemigo->posicion.x = nuevaX;
        enemigo->posicion.y = nuevaY;
    }
}
void muerteEnemigo(Enemigo* enemigos, int numEnemigos) {
    for (int i = 0; i < numEnemigos; i++) {
        if (enemigos[i].vida <= 0) {  
            enemigos[i].vida = 50;  
            enemigos[i].posicion = (Vector2){ GetRandomValue(-1000, 19200), GetRandomValue(-1000, 10800) }; 
        }
    }
}