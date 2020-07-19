// Indica que este archivo solo se debe incluir una vez en la compilación
#pragma once
#include "../imagelib/imagelib.h"

// Los objetos que queremos guardar en el quadtree

struct Arbol;
typedef struct Arbol Arbol;
struct Arbol{
  int x0;
  int z0;
  int x1;
  int z1;
  double gamma;
  int nivel;
  Color media_color;
  Arbol* hijo_Norte;
  Arbol* hijo_Sur;
  Arbol* hijo_Este;
  Arbol* hijo_Oeste;
};

void Insertar(Arbol** arbol, int x0,int z0,int x1,int z1,int nvl, Image* img);
void Insertar_filto(Arbol** arbol, int x0,int z0,int x1,int z1,int nvl, Image* img, double alpha);
void Alturas(Arbol* arbol);
int altura(Arbol *arbol, int nvl);
void destroy_Norte(Arbol* arbol);
void destroy_Sur(Arbol* arbol);
void destroy_Este(Arbol* arbol);
void destroy_Oeste(Arbol* arbol);
void destroy(Arbol* arbol);