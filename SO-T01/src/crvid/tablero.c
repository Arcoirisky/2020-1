#include "tablero.h"
#include <stdio.h>
#include <stdlib.h>

// init del tablero
Tablero* init_tablero(int D, int Linea)
{
  Tablero* tab = malloc(sizeof(Tablero));
  tab -> alto = D;
  tab -> ancho = D;
  
  // cada fila es un arrey de ints
  tab -> tablero = malloc(D * sizeof(int*));
  for(int fila = 0; fila < D; fila++)
  {
    // el tablero es cuadrado DxD
    tab -> tablero[fila] = malloc(D * sizeof(int));
    for(int col = 0; col < D; col++)
    {
      // A cada celda le asignamos 0 inicialmente
      tab -> tablero[fila][col] = 0;
    }
  }
  return tab;
}

// Imprime el tablero
void imprimir_Tablero(Tablero* tab)
{
  for(int fila = 0; fila < tab -> alto; fila++)
  {
    for(int col = 0; col < tab -> ancho; col++)
    {
      printf("%i ",tab -> tablero[fila][col]);
    }
    printf("\n");
  }
  printf("\n---------------\n");
}

// Free tablero
void destroy_Tablero(Tablero* tab)
{
  for(int fila = 0; fila < tab -> alto; fila++)
  {
    // libero las filas
    free(tab -> tablero[fila]);
  }
  // libero el tablero
  free(tab -> tablero);
  // libero la estructura del tablero
  free(tab);
}

int recorrerCentros(Tablero* mesa, int i, int j){
  // i == x; j == y
  int vecinos = 0;
  // fila superior
  if (mesa -> tablero[i-1][j-1] == 1){
    vecinos++;
  }
  if (mesa -> tablero[i-1][j] == 1){
    vecinos++;
  }
  if (mesa -> tablero[i-1][j+1] == 1){
    vecinos++;
  }
  // fila propia
  if (mesa -> tablero[i][j-1] == 1){
    vecinos++;
  }
  if (mesa -> tablero[i][j+1] == 1){
    vecinos++;
  }
  // fila inferior
  if (mesa -> tablero[i+1][j-1] == 1){
    vecinos++;
  }
  if (mesa -> tablero[i+1][j] == 1){
    vecinos++;
  }
  if (mesa -> tablero[i+1][j+1] == 1){
    vecinos++;
  }
  return vecinos;
}

int esquina(Tablero* mesa, int x, int y){
  // caso 00
  if(x == 0){
    if(y == 0){
    return (mesa -> tablero[x][y+1]) + (mesa -> tablero[x+1][y+1]) + (mesa -> tablero[x+1][y]);
    return 0;
    }
  }
  // caso 0D
  if(x == 0){
    if(y == mesa -> ancho-1){
    return (mesa -> tablero[x][y-1]) + (mesa -> tablero[x+1][y-1]) + (mesa -> tablero[x+1][y]);
    }
  }
  // caso D0
  if(x == mesa ->alto-1){
    if(y == 0){
    return (mesa -> tablero[x-1][y]) + (mesa -> tablero[x-1][y+1]) + (mesa -> tablero[x][y+1]);
    }
  }
  // caso DD
  if(x == mesa -> alto-1){
    if(y == mesa -> ancho-1){
    return (mesa -> tablero[x][y-1]) + (mesa -> tablero[x-1][y-1]) + (mesa -> tablero[x-1][y]);
    }
  }
}

int lineasup(int x, int y, Tablero* mesa){
   return mesa -> tablero[x][y+1]+ mesa -> tablero[x+1][y+1]+ mesa -> tablero[x+1][y]+ mesa -> tablero[x+1][y-1]+ mesa -> tablero[x][y-1];
}

int lineainf(int x, int y, Tablero* mesa){
   return mesa -> tablero[x-1][y-1]+ mesa -> tablero[x][y-1]+ mesa -> tablero[x-1][y]+ mesa -> tablero[x-1][y+1]+ mesa -> tablero[x][y+1];
}

int lineaizq(int i, int j,Tablero* mesa){
  return mesa -> tablero[i-1][j]+ mesa -> tablero[i-1][j+1]+ mesa -> tablero[i][j+1]+ mesa -> tablero[i+1][j]+ mesa -> tablero[i+1][j+1];
}

int lineader(int i, int j,Tablero* mesa){
  return mesa -> tablero[i-1][j-1]+ mesa -> tablero[i-1][j]+ mesa -> tablero[i][j-1]+ mesa -> tablero[i+1][j-1]+ mesa -> tablero[i+1][j];
}