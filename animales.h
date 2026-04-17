#ifndef ANIMALES_H
#define ANIMALES_H

#include "raylib.h"

typedef struct Animales {
	Vector2 posicion;
	int velocidad;
	Texture2D texturaAnimal;
	float vida;
	Vector2 direccion;
	float tiempoDesdeCambio;
	float escala;
} Animales;

void InicializarAnimales(Animales* animales);
void ActualizarAnimales(Animales* animales, float deltaTime);
void muerteAnimales(Animales* animales);

#endif //ANIMALES_H