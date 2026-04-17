// ui.c
#include "utilidades.h"

void DibujarBarraDeVida(Vector2 posicion, float vidaActual, float vidaMaxima) {
    float anchoBarra = 50.0f;
    float altoBarra = 5.0f;

    Vector2 posicionBarra = { posicion.x - anchoBarra / 2, posicion.y - 20 };

    // Aseguramos que los valores sean flotantes
    float vidaRatio = (float)vidaActual / (float)vidaMaxima;
    float anchoVidaActual = anchoBarra * vidaRatio;
    // Asegurarse de que la vida no sea negativa
    if (anchoVidaActual < 0) anchoVidaActual = 0;

    // Dibujar el fondo (barra roja)
    DrawRectangle(posicionBarra.x, posicionBarra.y, anchoBarra, altoBarra, RED);

    // Dibujar la vida actual (barra verde) solo si hay vida
    if (anchoVidaActual > 0) {
        DrawRectangle(posicionBarra.x, posicionBarra.y, anchoVidaActual, altoBarra, GREEN);
    }
}