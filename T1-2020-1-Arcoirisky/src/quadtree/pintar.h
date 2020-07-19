// Indica que este archivo solo se debe incluir una vez en la compilación
#pragma once
#include "../imagelib/imagelib.h"
#include "pintar.h"
#include "arbol.h"

void pintar_arbol(Arbol* arbol, double alpha, Image* img);
void contar_hojas(Arbol* arbol, double alpha, int* contador);
