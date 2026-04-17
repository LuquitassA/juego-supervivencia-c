#ifndef CAMARA_H
#define CAMARA_H

#include "raylib.h"
#include "jugador.h"

void InicializarCamara(Camera2D* camara);
void ActualizarCamara(Camera2D* camara, Jugador* jugador);

#endif // CAMARA_H

