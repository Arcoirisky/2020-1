#include "structs.h"
#include "ingreso_terminal.h"
#include "clausura.h"
#include <stdlib.h>
#include <stdio.h>
void meter_al_ente(Titanic* terminales, int ter1, int ter2,int p, int indice){

  Persona* persona_aux = terminales -> station[ter1] -> lista -> door[p] -> cola -> people[indice];
  int priority = persona_aux ->tipo;
  int posicion = 0;
  int* pos = &posicion;
  int eleccion_puerta = elegir_puerta(terminales, ter2, priority, pos);
      
  insertar_cola(terminales -> station[ter2] -> lista -> door, eleccion_puerta,persona_aux, posicion);
  
  int largo_laser = terminales -> station[ter1] -> lista -> door[p] -> cola -> largo;
  for(int continua = indice; continua < largo_laser -1; continua++){
    terminales->station[ter1]->lista->door[p]->cola->people[continua]=terminales->station[ter1]->lista->door[p]->cola-> people[continua + 1];
  }
  //imprimir_cola(terminales->station[ter1]->lista->door[p]->cola);
  terminales -> station[ter1] -> lista -> door[p] -> cola -> largo -= 1;
}


void meter_entes(Titanic* terminales, int ter1, int ter2, int tipo){
  int puertas_ter1 = terminales -> station[ter1] -> lista -> largo;
  int Cant[puertas_ter1][3];
  for(int puerta = 0; puerta < puertas_ter1; puerta++){
    // identifico cuanta gente de cada tipo existe en cada puerta del terminal elegido
    int Cantidad[3] = {cantidad_ente(terminales -> station[ter1] -> lista -> door[puerta] -> cola, 0),
                       cantidad_ente(terminales -> station[ter1] -> lista -> door[puerta] -> cola, 1),
                       cantidad_ente(terminales -> station[ter1] -> lista -> door[puerta] -> cola, 2)
                      };
    for(int op = 0; op < 3; op++){ //3 es por los 3 tipos
      //printf("%i %i %i\n", Cantidad[0],Cantidad[1],Cantidad[2]);
      Cant[puerta][op] = Cantidad[op]; 
      //Cant[Puerta][ninos,adultos,robots]
    } 
  }
  
  
  int entes = 0;
  for(int i=0; i<puertas_ter1; i++){
    entes += Cant[i][tipo];
  }
  
  while (entes != 0){
    for(int puerta_elegida = 0; puerta_elegida < puertas_ter1; puerta_elegida++){
      if(Cant[puerta_elegida][tipo] != 0){
        meter_al_ente(terminales, ter1, ter2, puerta_elegida, 0);
        Cant[puerta_elegida][tipo] -= 1;     
      }
    }
    entes = 0;
    for(int i=0; i<puertas_ter1; i++){
    entes += Cant[i][tipo];
    }
  }
  
}



























