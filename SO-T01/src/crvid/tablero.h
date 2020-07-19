// Indica que este archivo solo se debe incluir una vez en la compilación
#pragma once

struct Tablero;
// Definimos un alias para el struct matriz.
// La sintaxis es "typedef tipo alias"
typedef struct Tablero Tablero;

// Definimos el struct matriz
struct Tablero
{
  int** tablero;
  int alto;
  int ancho;
};

// Declaramos las funciones asociadas

Tablero* init_tablero(int D, int Linea);
void imprimir_Tablero(Tablero* tab);
void destroy_Tablero(Tablero* tab);
int recorrerCentros(Tablero* mesa, int i, int j);
int esquina(Tablero* mesa, int x, int y);
int lineasup(int x, int y, Tablero* mesa);
int lineainf(int x, int y, Tablero* mesa);
int lineader(int x, int y,Tablero* mesa);
int lineaizq(int x, int y,Tablero* mesa);