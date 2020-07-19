#include "structs.h"
#include "crearProcesos.h"
#include <stdlib.h>
#include <stdio.h>
int sumaBusts(Proceso* process){
  int n = 0;
  for(int k = 0; k < process ->List_rafagas ->largo; k++){
    n += process -> List_rafagas ->burst[k] ->CPU_Bust;
  }
  return n;
}

int elegir_continua(COLA* ready){

  int n = sumaBusts(ready -> process[0]);
  int menor = 0;
  for(int j = 0; j < ready -> largo; j++){
    int actual = sumaBusts(ready -> process[j]);
    if (actual < n){
      n = actual;
      menor = j;
    }
    else{
      if (actual == n && menor != j){
        int actual_inm = ready -> process[j] ->List_rafagas ->burst[0] ->CPU_Bust;
        int menor_inm = ready -> process[menor] ->List_rafagas ->burst[0] ->CPU_Bust;
        if (actual_inm < menor_inm){
          n = actual;
          menor = j;
        }
      }
    }
  }
  return menor;
}

void sacar_proceso_ready_to_cpu(COLA* ready, CPU* cpu, int t_sim){
  // para saber cual proceso sacare pueden pasar 2 cosas

  if (ready -> largo == 1){
    //Solo tenemos un proceso en ready
    free(cpu -> process); //Anda a saber tu, lo debo liberar antes de meterle otro proceso
    cpu -> process = clonar_proceso(ready -> process[0]);
    if(cpu ->process->t_cpu<0){
      cpu ->process->t_cpu = t_sim;
    }
    cpu -> process -> estado = 2;
    cpu ->process ->turnos_cpu++;
    cpu -> estado = -1;
    //printf("INGRESANDO PROCESO %s EN CPU en t=%i\n", cpu -> process -> name,t_sim);
    laser_cola(ready, 0);
  }
  else{
    // mas de un proceso en cola...
    // Entonces debo elegir el proceso com menor CPU bust
    int elegido = elegir_continua(ready);
    free(cpu -> process); //Anda a saber tu, lo debo liberar antes de meterle otro proceso
    cpu -> process = clonar_proceso(ready -> process[elegido]);
    if(cpu ->process->t_cpu<0){
      cpu ->process->t_cpu = t_sim;
    }
    cpu -> process -> estado = 2;
    cpu ->process ->turnos_cpu++;
    cpu -> estado = -1;
    //printf("INGRESANDO PROCESO %s EN CPU en t=%i\n", cpu -> process -> name, t_sim);
    laser_cola(ready, elegido);

  }
}
