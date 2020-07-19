#include "structs.h"
#include <stdlib.h>
#include <stdio.h>

void ingresar_in_cola(COLA* mod_cola, Proceso* new_process){
  int tengo = mod_cola -> largo;
  int capacidad = mod_cola -> capacidad;
  if (tengo < capacidad){
        mod_cola -> process[tengo] = new_process;
        mod_cola -> largo += 1;
  } 
  else{ //no tengo más espacio:
      mod_cola -> process = realloc(mod_cola -> process,sizeof(Proceso*)*((mod_cola -> capacidad)+10));
      mod_cola -> capacidad += 10;
      ingresar_in_cola(mod_cola, new_process);
    }
}

void destroy_Proceso_IDLE(IDLE* NoCreados, int index){
  for(int k=0;k< NoCreados -> process[index] -> List_rafagas -> largo; k++){
    free(NoCreados -> process[index] -> List_rafagas -> burst[k]);
  }
  free(NoCreados -> process[index] -> List_rafagas -> burst);
  free(NoCreados -> process[index] ->List_rafagas);
  free(NoCreados -> process[index]);
}

void laser_idle(IDLE* NoCreados, int index){
    destroy_Proceso_IDLE(NoCreados, index);
    int largo_laser = NoCreados -> largo;
    for(int continua = index; continua < largo_laser - 1; continua++){
        NoCreados -> process[continua] = NoCreados -> process[continua + 1];
    }
    NoCreados -> largo -= 1;
}

void destroy_Proceso_COLA(COLA* cola, int index){
  for(int k=0;k< cola -> process[index] -> List_rafagas -> largo; k++){
    free(cola -> process[index] -> List_rafagas -> burst[k]);
  }
  free(cola -> process[index] -> List_rafagas -> burst);
  free(cola -> process[index] -> List_rafagas);
  free(cola -> process[index]);
}

void laser_cola(COLA* cola, int index){
  destroy_Proceso_COLA(cola, index);
  int largo_laser = cola -> largo;
  for(int continua = index; continua < largo_laser - 1; continua++){
      cola -> process[continua] = cola -> process[continua + 1];
  }
  cola -> largo -= 1;
}

Proceso* crear_proceso(IDLE* NoCreados, COLA* ready,int t0){
  int i = 0;
  while(i < NoCreados -> largo){
    if (NoCreados -> process[i] -> t_0 == t0){
      Proceso* new_process = clonar_proceso(NoCreados -> process[i]);
      new_process -> estado = 1; // 1 == READY_Mode
      ingresar_in_cola(ready, new_process);
      laser_idle(NoCreados, i);
      //printf("PROCESO %s creado! en t=%i\n", new_process -> name, t0);
      i = -1;
    }
    i++;
  }
}