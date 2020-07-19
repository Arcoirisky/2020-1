#include "structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

IDLE* NoCreados_Init(char* input){
  // Abrimos el archivo
  FILE* file = fopen(input,"r");
  // Leemos la cantidad de Procesos
  int process_count;
  fscanf(file, "%i", &process_count);
  //printf("tengo %i procesos a crear\n",process_count);
  // Creo structura IDLE de procesos no creados
  IDLE* NoCreados = malloc(sizeof(IDLE));
  NoCreados -> largo = process_count;
  NoCreados -> total = process_count;
  NoCreados -> process = malloc(sizeof(Proceso*)* NoCreados -> largo);

  for(int Aux = 0; Aux < NoCreados ->largo; Aux++){
    //Sacamos el nombre, t0 y la cantidad de CPU-Busts
    char nombre[32];
    int t0;
    int N;
    fscanf(file, "%s %i %i", nombre, &t0, &N);
    //printf("creando proceso %s inicia en %i con %i busts\n",nombre, t0, N);
    Lista* new_listaRafagas = malloc(sizeof(Lista));
    new_listaRafagas -> largo = N;
    new_listaRafagas -> burst = malloc(sizeof(Rafaga*)*N);

    for(int i = 0; i<N;i++){
      Rafaga* new_rafaga = malloc(sizeof(Rafaga));
      int CPU;
      fscanf(file, "%i", &CPU);
      new_rafaga -> CPU_Bust = CPU;
      if (N > 1 && i < N-1){
        int IO;
        fscanf(file, "%i", &IO);
        new_rafaga -> IO_Bust = IO;
      }
      else{
        new_rafaga -> IO_Bust = 0;
      }
      new_listaRafagas -> burst[i] = new_rafaga;
    }
    // meterlo dentro de un proceso
    Proceso* new_proceso = malloc(sizeof(Proceso));
    strcpy(new_proceso -> name, nombre);
    new_proceso -> t_0 = t0;
    new_proceso -> estado = 0; // No creado
    new_proceso -> List_rafagas = new_listaRafagas;
    new_proceso -> WaitingTime = 0;
    new_proceso -> t_cpu = -1;
    new_proceso -> t_f = -1;
    new_proceso -> turnos_cpu = 0;
    new_proceso -> t_rupcion = 0;
    //printf("agregado a idle el proceso %s wii\n",new_proceso -> name);
    NoCreados -> process[Aux] = new_proceso;
  }
  fclose(file);
  return NoCreados;
}

Lista* clonar_listaRafagas(Lista* old_listaRafagas){
    Lista* new_listaRafagas = malloc(sizeof(Lista));
    new_listaRafagas -> largo = old_listaRafagas ->largo ;
    new_listaRafagas -> burst = malloc(sizeof(Rafaga*)* new_listaRafagas -> largo);

    for(int i = 0; i< new_listaRafagas -> largo;i++){
      Rafaga* new_rafaga = malloc(sizeof(Rafaga));
      new_rafaga -> CPU_Bust = old_listaRafagas -> burst[i] ->CPU_Bust;
      new_rafaga -> IO_Bust = old_listaRafagas -> burst[i] ->IO_Bust;

      new_listaRafagas -> burst[i] = new_rafaga;
    }
    return new_listaRafagas;
}

Proceso* clonar_proceso(Proceso* old_proceso){
    Proceso* new_proceso = malloc(sizeof(Proceso));
    strcpy(new_proceso -> name, old_proceso -> name);
    new_proceso -> t_0 = old_proceso -> t_0;
    new_proceso -> estado = old_proceso -> estado; // No creado
    new_proceso -> List_rafagas = clonar_listaRafagas(old_proceso -> List_rafagas);
    new_proceso -> WaitingTime = old_proceso -> WaitingTime;
    new_proceso -> t_cpu = old_proceso -> t_cpu;
    new_proceso -> t_f = old_proceso -> t_f;
    new_proceso -> turnos_cpu = old_proceso -> turnos_cpu;
    new_proceso -> t_rupcion = old_proceso -> t_rupcion;
    return new_proceso;
}

COLA* cola_init(){
    COLA* cola = malloc(sizeof(COLA));
    cola -> largo = 0;
    cola -> capacidad = 5;
    cola -> process = malloc(sizeof(Proceso*)*cola -> capacidad);
    return cola;
}

CPU* cpu_init(){
    CPU* cpu = malloc(sizeof(CPU));
    cpu -> estado = 0; // Libre
    cpu -> process = malloc(sizeof(Proceso*));
    return cpu;
}

void destroyProcessCPU(CPU* cpu){
    for(int k=0;k< cpu -> process -> List_rafagas -> largo; k++){
        free(cpu -> process-> List_rafagas -> burst[k]);
    }
    free(cpu -> process -> List_rafagas -> burst);
    free(cpu -> process -> List_rafagas);
}

void destroyCPU(CPU* cpu){
    if(cpu -> estado != 0){
        for(int k=0;k< cpu -> process -> List_rafagas -> largo; k++){
            free(cpu -> process-> List_rafagas -> burst[k]);
        }
        free(cpu -> process -> List_rafagas -> burst);
        free(cpu -> process -> List_rafagas);
    }
    free(cpu -> process);
    free(cpu);
}

void destroyIDLE(IDLE* NoCreados){
    for(int Proc = 0; Proc < NoCreados ->largo; Proc++){
        for(int k=0;k< NoCreados -> process[Proc] -> List_rafagas -> largo; k++){
            free(NoCreados -> process[Proc] -> List_rafagas -> burst[k]);
        }
        free(NoCreados -> process[Proc] -> List_rafagas -> burst);
        free(NoCreados -> process[Proc] ->List_rafagas);
        free(NoCreados -> process[Proc]);
    }
    free(NoCreados -> process);
    free(NoCreados); 
}

void destroyCola(COLA* cola){
    for(int Proc = 0; Proc < cola ->largo; Proc++){
        for(int k=0;k< cola -> process[Proc] -> List_rafagas -> largo; k++){
            free(cola -> process[Proc] -> List_rafagas -> burst[k]);
        }
        free(cola -> process[Proc] -> List_rafagas -> burst);
        free(cola -> process[Proc] -> List_rafagas);
        free(cola -> process[Proc]);
    }
    free(cola -> process);
    free(cola); 
}