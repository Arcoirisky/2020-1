#include "structs.h"
#include "crearProcesos.h"
#include "cpuFunctions.h"
#include "actualizadores.h"
#include "estadisticas.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool string_equals(char* string1, char* string2)
{
  return !strcmp(string1, string2);
}

/*
  Estados:
  0 - inexistente
  1 - ready
  2 - running
  3 - waiting
  4 - finish
*/

void non_preemptive(char* input, char* output){
  //printf("NON-PREEMPTIVE SCHEDULER SIMULATOR\n");
  IDLE* NoCreados = NoCreados_Init(input);
  COLA* Ready = cola_init();
  COLA* Waiting = cola_init();
  COLA* Finish = cola_init();
  CPU* cpu = cpu_init();
  int total = NoCreados->largo;
  int t_sim = 0; //Este será el que me controle el tiempo de la simulacion
  //printf("------------------------------\n");
  // En el caso de que aún queden procesos por crear
  while (Finish ->largo<total){
    //printf("T: %i\n",t_sim);

    if(NoCreados -> largo > 0){ 
      crear_proceso(NoCreados, Ready,t_sim);
    }

    // CPU libre y procesos en Ready
    if (cpu -> estado == 0 && Ready -> largo > 0){
      //De todos los procesos en Ready, sacar uno y mandarlo a la CPU en modo running
      sacar_proceso_ready_to_cpu(Ready, cpu, t_sim);
    }

    // Actualizar tiempos:
    // WaitingTime += 1 a todo proceso en ready
    actualizar_t_ready(Ready);
    // CPU-Bust -= 1 al proceso en CPU
    actualizar_t_cpu(cpu);
    // WaitingTime += 1 a todo proceso en waiting
    // IO-Bust -= 1 a todo proceso en waiting
    actualizar_t_waiting(Waiting);
    
    
    // Actualizar estados
    /*
    - En CPU: si CPU-Bust es 0 entonces reviso el largo de las rafagas.
    Si este largo == 1 entonces el proceso pasa a estar en estado finish. y registro t de salida
    Si este largo > 1 entonces el proceso debe tener un waiting mode y 
      se saca de la CPU para entrar en la cola waiting.
    */
    //En waiting: si IO-Bust es 0 entonces borro la rafaga de la lista de rafagas del proceso
    //  y paso el proceso a waiting_Mode
    actualizar_waiting(Waiting, Ready, t_sim);
    
    actualizar_cpu(cpu, Finish, Waiting, t_sim);
    //actualizar_cpu(cpu, Finish, Waiting, t_sim);
    
    t_sim++;
  }
  // Escribir archivo output
  escribir_salida_np(Finish, output);

  //printf("--------------------------\n");
  // Esto es momentaneo en lo que no tengo más listas, por ahora todo está en NoCreados
  //printf("Liberando proceso de NoCreados %i...\n",NoCreados->largo);
  destroyIDLE(NoCreados);
  //printf("Liberando proceso de Ready %i...\n",Ready->largo);
  destroyCola(Ready);
  //printf("Liberando proceso de Waiting %i...\n",Waiting->largo);
  destroyCola(Waiting);
  //printf("Liberando proceso de Finish %i...\n",Finish->largo);
  destroyCola(Finish);
  //printf("Liberando CPU %i\n",cpu->estado);
  destroyCPU(cpu);
}

void preemptive(char* input, char* output, int quantum){
  //printf("PREEMPTIVE SCHEDULER SIMULATOR with q = %i\n", quantum);
  IDLE* NoCreados = NoCreados_Init(input);
  COLA* Ready = cola_init();
  COLA* Waiting = cola_init();
  COLA* Finish = cola_init();
  CPU* cpu = cpu_init();
  int total = NoCreados->largo;
  int t_sim = 0; //Este será el que me controle el tiempo de la simulacion
  //printf("------------------------------\n");
  int aux_q = 0;
  int* p_aux_q = &aux_q; //para poder modificarlo

  while(Finish->largo < total){
    // En el caso de que aún queden procesos por crear
    //printf("T= %i q_aux= %i\n",t_sim, *p_aux_q);    
    if(NoCreados -> largo > 0){ 
      crear_proceso(NoCreados, Ready,t_sim);
    }
    if (cpu -> estado == 0 && Ready -> largo > 0){
      //De todos los procesos en Ready, sacar uno y mandarlo a la CPU en modo running
      sacar_proceso_ready_to_cpu(Ready, cpu, t_sim);
    }

    //En principio todo actua normal hasta que se cumpla el quantum.
    // El quantum afecta al proceso en la CPU
    // Actualizar tiempos:
    actualizar_t_ready(Ready);
    actualizar_t_cpu(cpu);
    actualizar_t_waiting(Waiting);
    
    // Todo se actualiza de forma normal
    
    //printf("%i == %i de q %i\n",*p_aux_q, aux_q, quantum);
    // Actualizar estados
    if (cpu ->estado != 0){
      *p_aux_q += 1;
    }
    actualizar_waiting(Waiting, Ready, t_sim);
    actualizar_cpu_2(cpu, Finish, Waiting, Ready, t_sim, p_aux_q, quantum); //<-- here
    // Waitingtime actua normal
    
    if(*p_aux_q >= quantum){
      *p_aux_q = 0;
    }
    t_sim++;
  }
  escribir_salida_p(Finish, output);
  //printf("--------------------------\n");
  // Esto es momentaneo en lo que no tengo más listas, por ahora todo está en NoCreados
  //printf("Liberando proceso de NoCreados %i...\n",NoCreados->largo);
  destroyIDLE(NoCreados);
  //printf("Liberando proceso de Ready %i...\n",Ready->largo);
  destroyCola(Ready);
  //printf("Liberando proceso de Waiting %i...\n",Waiting->largo);
  destroyCola(Waiting);
  //printf("Liberando proceso de Finish %i...\n",Finish->largo);
  destroyCola(Finish);
  //printf("Liberando CPU %i\n",cpu->estado);
  destroyCPU(cpu);
}

int main(int argc, char *argv[])
{
  // Este programa recibe tres parametros:
  //  argv[0] = el programa en si
  //  argv[1] = la ruta al archivo de input
  //  argv[2] = la ruta del archivo output
  //  argv[3] = version (np/p)
  //  argv[4] = quantum
  if (argc > 5 || argc < 4)
  {
    printf("Cantidad de parámetros incorrecta\n");
    printf("Uso correcto: %s PATH_TO_INPUT PATH_TO_OUTPUT np/p quantum\n", argv[0]);
    return 1;
  }
  else{
    if (string_equals(argv[3], "np")){ 
      //non-preemptive
      char* filename_In = argv[1];
      char* filename_Out = argv[2];
      non_preemptive(filename_In, filename_Out);
    }
    else{
      // preemptive
      int quantum = 5;
      if (argc == 5) {
        quantum = atoi(argv[4]);
      }
      char* filename_In = argv[1];
      char* filename_Out = argv[2];
      preemptive(filename_In, filename_Out, quantum);
    }
  }
  return 0;
}
