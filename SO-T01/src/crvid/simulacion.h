#pragma once

#include "tablero.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Indica que este archivo solo se debe incluir una vez en la compilación

void juegoDeLaVida(int iteraciones,int A,int B,int C,int D,int tablero, int lineaproceso);
void agregarcambio(Tablero* aux, Tablero* mesa, int vecinos, int x, int y,int A, int B, int C);
void llenarTablero(Tablero* mesa, int linea_tablero);
void actualizarmesa(Tablero* mesa, Tablero* aux);
bool celulasVivas(Tablero* mesa);
int contarCelulas(Tablero* mesa);
