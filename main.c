#include "raylib.h"
#include "mapa.h"
#include "jugador.h"
#include "camara.h"
#include "enemigo.h"
#include "animales.h"
#include "utilidades.h"
#include "recursos.h"
#include "refugio.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define NUM_ENEMIGOS 300

void DibujarMenu() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    int fontSize = 40;
    int spacing = 50;

    DrawText("Juego de Supervivencia", SCREEN_WIDTH / 2 - MeasureText("Juego de Supervivencia", fontSize) / 2, 100, fontSize, BLACK);

    DrawText("1. Jugar", SCREEN_WIDTH / 2 - MeasureText("1. Jugar", fontSize) / 2, 250, fontSize, BLACK);
    DrawText("2. Instrucciones", SCREEN_WIDTH / 2 - MeasureText("2. Instrucciones", fontSize) / 2, 250 + spacing, fontSize, BLACK);
    EndDrawing();
}

void MostrarInstrucciones() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    int fontSize = 20;
    int yOffset = 100;
    int lineSpacing = 30;

    DrawText("Controles:", SCREEN_WIDTH / 2 - MeasureText("Controles:", fontSize + 10) / 2, yOffset, fontSize + 10, BLACK);

    DrawText("WASD - Movimiento", SCREEN_WIDTH / 2 - MeasureText("WASD - Movimiento", fontSize) / 2, yOffset + lineSpacing, fontSize, BLACK);
    DrawText("SHIFT - Correr", SCREEN_WIDTH / 2 - MeasureText("SHIFT - Correr", fontSize) / 2, yOffset + lineSpacing * 2, fontSize, BLACK);
    DrawText("CTRL - Caminar lento", SCREEN_WIDTH / 2 - MeasureText("CTRL - Caminar lento", fontSize) / 2, yOffset + lineSpacing * 3, fontSize, BLACK);
    DrawText("ESPACIO - Atacar", SCREEN_WIDTH / 2 - MeasureText("ESPACIO - Atacar", fontSize) / 2, yOffset + lineSpacing * 4, fontSize, BLACK);
    DrawText("E - Recolectar recursos", SCREEN_WIDTH / 2 - MeasureText("E - Recolectar recursos", fontSize) / 2, yOffset + lineSpacing * 5, fontSize, BLACK);
    DrawText("B - Construir refugio", SCREEN_WIDTH / 2 - MeasureText("B - Construir refugio", fontSize) / 2, yOffset + lineSpacing * 6, fontSize, BLACK);

    DrawText("Presiona ENTER para volver", SCREEN_WIDTH / 2 - MeasureText("Presiona ENTER para volver", fontSize) / 2, SCREEN_HEIGHT - 100, fontSize, DARKGRAY);
    EndDrawing();
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Juego de Supervivencia");
    SetTargetFPS(60);

    // Estado del menú
    enum GameState {
        MENU,
        INSTRUCTIONS,
        GAME
    };

    enum GameState currentState = MENU;

    // Variables para el juego (inicialízalas fuera del bucle del juego)
    Camera2D camara;
    Jugador Jugador;
    Refugios refugios;
    Recursos recursos;
    Enemigo enemigos[NUM_ENEMIGOS];
    Animales animales;

    while (!WindowShouldClose())
    {
        switch (currentState) {
        case MENU:
            DibujarMenu();
            if (IsKeyPressed(KEY_ONE)) {
                // Inicializar todo para el juego
                InicializarCamara(&camara);
                InicializarJugador(&Jugador);
                InicializarInventario(&Jugador);
                InicializarRefugios(&refugios);
                InicializarRecursos(&recursos);
                InicializarEnemigos(enemigos, NUM_ENEMIGOS);
                InicializarAnimales(&animales);
                InicializarMapa();
                currentState = GAME;
            }
            else if (IsKeyPressed(KEY_TWO)) {
                currentState = INSTRUCTIONS;
            }
            break;

        case INSTRUCTIONS:
            MostrarInstrucciones();
            if (IsKeyPressed(KEY_ENTER)) {
                currentState = MENU;
            }
            break;

        case GAME:

        {
            while (!WindowShouldClose())
            {
                float deltaTime = GetFrameTime();

                // Actualizar
                MoverJugador(&Jugador);
                AplicarDaño(&Jugador, &animales, 1, enemigos, NUM_ENEMIGOS);
                ActualizarEnemigos(enemigos, NUM_ENEMIGOS, deltaTime);
                AplicarDañoAlJugador(&Jugador, &enemigos, NUM_ENEMIGOS, deltaTime);
                muerteEnemigo(enemigos, NUM_ENEMIGOS);
                muerteAnimales(&animales);
                ActualizarAnimales(&animales, deltaTime);
                ActualizarCamara(&camara, &Jugador);
                DibujarRecursos(&recursos);
                // uso del inventario
                interaccionInventario(&Jugador);
                //recoleccion de materiales
                if (IsKeyDown(KEY_E)) {
                    RecolectarRecurso(&recursos, &Jugador);
                }
                if (IsKeyPressed(KEY_B)) {  // 'B' para construir refugio
                    CrearRefugio(&refugios, &Jugador);
                }

                // No olvides dibujar los refugios

                // Dibujar
                BeginDrawing();
                ClearBackground(RAYWHITE);

                BeginMode2D(camara);
                DibujarMapa();
                DibujarRecursos(&recursos);
                DibujarRefugios(&refugios);


                float escalaJugador = 0.12f;
                Vector2 jugadorCentro = (Vector2){
                    Jugador.posicion.x - (Jugador.textura.width * escalaJugador) / 2,
                    Jugador.posicion.y - (Jugador.textura.height * escalaJugador) / 2
                };
                DrawTextureEx(Jugador.textura, jugadorCentro, 0.0f, escalaJugador, WHITE);
                DibujarBarraDeVida(Jugador.posicion, Jugador.vida, 100.0f);

                // Dibujar enemigos
                for (int i = 0; i < NUM_ENEMIGOS; i++) {
                    Vector2 enemigoCentro = (Vector2){
                        enemigos[i].posicion.x - (enemigos[i].texturaEnemigo.width * enemigos[i].escala) / 2,
                        enemigos[i].posicion.y - (enemigos[i].texturaEnemigo.height * enemigos[i].escala) / 2
                    };
                    DrawTextureEx(enemigos[i].texturaEnemigo, enemigoCentro, 0.0f, enemigos[i].escala, WHITE);
                    DibujarBarraDeVida(enemigos[i].posicion, enemigos[i].vida, 50.0f);
                }

                // Dibujar animales
                DrawTextureEx(animales.texturaAnimal, animales.posicion, 0.0f, animales.escala, WHITE);
                DibujarBarraDeVida(animales.posicion, animales.vida, 25.0f);

                DibujarAreaAtaque(Jugador);

                EndMode2D();

                // Dibujar inventario fuera del modo 2D
                DibujarInventario(&Jugador);


                EndDrawing();
            }
        }
        break;
        }
    }

    CloseWindow();
    return 0;
}