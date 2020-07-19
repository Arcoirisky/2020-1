// Indica que este archivo solo se debe incluir una vez en la compilación
#pragma once
#include "../imagelib/imagelib.h"

void promedio_cuadrante(Image* img, int x0, int z0,int x1, int z1, double* promL, double* promA, double* promB);
double calcular_gamma(int x0,int z0,int x1,int z1,Image* img);
Color promedio_Color(Image* img, int x0, int z0,int x1, int z1);