#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "raylib.h"

typedef struct Enemigo {
	Vector2 posicion;
	int velocidad;
	Texture2D texturaEnemigo;
	int vida;
	int daño;
	Vector2 direccion;
	float tiempoDesdeCambio;
	float escala;
	float rangoDeAtaque;
	float tiempoUltimoAtaque;  // Tiempo del último ataque
	float cooldownAtaque;

} Enemigo;


void InicializarEnemigos(Enemigo* enemigos, int numEnemigos);
void ActualizarEnemigos(Enemigo* enemigos, int numEnemigos, float deltaTime);


#endif // ENEMIGO_H