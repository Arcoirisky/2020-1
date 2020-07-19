#include "simulacion.h"
//#include "tablero.h"

void llenarTablero(Tablero* mesa, int linea_tablero){
  FILE* file = fopen("tableros.txt", "r");
  int contador = 0;
  while(contador < linea_tablero){
    int cantCelulas;
    fscanf(file, "%i", &cantCelulas);
    for(int aux = 0; aux < cantCelulas; aux++){
      int x,y;
      fscanf(file, "%i %i", &x, &y);
    }
    contador++;
  };
  int cantCelulas;
  fscanf(file, "%i", &cantCelulas);
  for(int aux = 0; aux < cantCelulas; aux++){
    int x,y;
    fscanf(file, "%i %i", &x, &y);
    mesa -> tablero[x][y] = 1;
  }
  fclose(file);
}

void actualizarmesa(Tablero* mesa, Tablero* aux){
  int D = mesa ->alto;
  for(int x=0;x<D;x++){
      for(int y=0; y<D; y++){
        mesa -> tablero[x][y] = aux -> tablero[x][y];
      }
    }
}

bool celulasVivas(Tablero* mesa){
  int D = mesa ->alto;
  for(int x=0;x<D;x++){
    for(int y=0; y<D; y++){
      if (mesa -> tablero[x][y] == 1) {
        return true;
      }
    }
  }
  return false;
}

int contarCelulas(Tablero* mesa){
  int D = mesa ->alto;
  int cant = 0;
  for(int x=0;x<D;x++){
    for(int y=0; y<D; y++){
      if (mesa -> tablero[x][y] == 1) {
        cant++;
      }
    }
  }
  return cant;
}

int sinal = 0;

void liberar(void){
  exit(0);
}

void ctrlCtablero(int sig){
  //printf("me apretaron ctl-C\n");
  sinal = 1;
  atexit(liberar);
}

void alarmatablero(int sig){
  //printf("me quede sin tiempo\n");
  sinal = 1;
  atexit(liberar);
}

void agregarcambio(Tablero* aux, Tablero* mesa, int vecinos, int x, int y,int A, int B, int C){
  
  if (vecinos == A){
    aux -> tablero[x][y] = 1;
  }
  else if (vecinos > C || vecinos < B){
    if(mesa -> tablero[x][y] == 1){
      aux -> tablero[x][y] = 0;
    }
    
  }
  else{
  aux -> tablero[x][y] = mesa -> tablero[x][y];
  }
}

void juegoDeLaVida(int iteraciones,int A,int B,int C,int D,int linea_tablero, int lineaproceso){
  signal(SIGINT, ctrlCtablero);
  signal(SIGALRM, alarmatablero);
  //signal (SIGALRM, alarmaSIMULACION);
  //printf("WOOO SIMULANDO JUEGO DE LA VIDA WOOOOH\n");
  //printf("Soy una simulacion\n");
  //printf("Mis datos son:\n - iteraciones: %i\n - A: %i\n - B: %i\n - C: %i\n - D: %i\n - Tablero: %i\n\n", iteraciones, A, B, C, D, linea_tablero);

  char nombrefile[255];
  sprintf(nombrefile, "%d.csv", lineaproceso);
  

  Tablero* mesa = init_tablero(D, linea_tablero);
  Tablero* aux = init_tablero(D, linea_tablero);
  llenarTablero(mesa, linea_tablero);
  llenarTablero(aux, linea_tablero);
  
  int count = 0;
  int vecinos = 0;
  while(count < iteraciones){
    //ahora tengo que revisar los vecinos e ir actualizando el tablero
    // Estos son los del centro
    ;
    if(sinal != 0){
      break;
    }
    //
    for(int x=1;x<D-1;x++){
      //printf("\n");
      for(int y=1; y<D-1; y++){
        vecinos = recorrerCentros(mesa,x,y);
        //printf("%i, (%i,%i) -- ", vecinos, x,y);
        agregarcambio(aux,mesa,vecinos,x, y, A,B,C);
      }
    }
    // Estos son las 4 esquinas
    // (0,0)
    
    vecinos = esquina(mesa,0,0);
    agregarcambio(aux,mesa,vecinos,0, 0, A,B,C);
    // (0,D-1)
    vecinos = esquina(mesa,0,D);
    agregarcambio(aux,mesa,vecinos,0, D-1, A,B,C);
    // (D-1,0)
    vecinos = esquina(mesa,D-1,0);
    agregarcambio(aux,mesa,vecinos,D-1, 0, A,B,C);
    // (D-1,D-1)
    vecinos = esquina(mesa,D-1,D-1);
    agregarcambio(aux,mesa,vecinos,D-1, D-1, A,B,C);
    // me faltan las orillas
    
    for(int j=1; j<D-1;j++){
      vecinos = lineasup(0,j,mesa);
      agregarcambio(aux,mesa,vecinos,0, j, A,B,C);
      vecinos = lineainf(D-1,j,mesa);
      agregarcambio(aux,mesa,vecinos,D-1, j, A,B,C);
    }
    for(int i=1; i<D-1;i++){
      vecinos = lineaizq(i,0,mesa);
      agregarcambio(aux,mesa,vecinos,i, 0, A,B,C);
      vecinos = lineader(i,D-1,mesa);
      agregarcambio(aux,mesa,vecinos,i, D-1, A,B,C);
    }
    
    actualizarmesa(mesa, aux);
    
    
    //sleep(10);
    count++;
    if (!celulasVivas(mesa)){
      //printf("MUERTE CELULAR simulacion %i\n",lineaproceso);
      //printf("0, %i, NOCELLS\n",count);
      FILE* file = fopen(nombrefile, "w");
      fprintf(file, "0, %i, NOCELLS\n",count);
      fclose(file);
      break;
    }
  };
  if(count >= iteraciones){
    if (celulasVivas(mesa)){    
      int cant_celulas = contarCelulas(mesa);
      //printf("MUERTE POR ITERACIONES simulacion %i\n",lineaproceso);
      FILE* file = fopen(nombrefile, "w");
      fprintf(file, "%i, %i, NOTTIME\n",cant_celulas, count);
      fclose(file);
      //imprimir_Tablero(mesa);
    }
  }
  if(sinal != 0){
    //printf("MUERTE POR SENIAAAAL simulacion %i\n",lineaproceso);
    int cantcelulas = contarCelulas(mesa);
    FILE* file = fopen(nombrefile, "w");
    fprintf(file, "%i, %i, SIGNAL\n",cantcelulas, count);
    fclose(file);
  }
  destroy_Tablero(mesa);
  destroy_Tablero(aux);
}
