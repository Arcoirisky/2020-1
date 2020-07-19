#include "structs.h"
#include "crearProcesos.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void actualizar_t_cpu(CPU* cpu){
  if (cpu ->estado != 0){
    //cpu ocupada entonces hay un proceso
    cpu -> process -> List_rafagas -> burst[0] -> CPU_Bust--;
  }
}

void actualizar_t_waiting(COLA* waiting){
  if (waiting ->largo > 0){
    for(int k = 0; k < waiting ->largo; k++){
      waiting ->process[k] -> WaitingTime++;
      waiting -> process[k] ->List_rafagas -> burst[0] ->IO_Bust--;
    }
  }
}

void actualizar_t_ready(COLA* ready){
  if (ready ->largo > 0){
    for(int r = 0; r<ready->largo; r++){
      ready ->process[r]-> WaitingTime++;
    }
  }
}

void laserRafagas(Proceso* process){
  //printf("laseando rafaga\n");
  free(process ->List_rafagas->burst[0]);
  int largo_laser = process->List_rafagas->largo;
  for(int continua=0; continua< largo_laser-1; continua++){  
    process ->List_rafagas ->burst[continua] = process->List_rafagas->burst[continua+1];
  }
  process ->List_rafagas -> largo--;
  
}

void actualizar_cpu_2(CPU* cpu, COLA* Finish, COLA* Waiting, COLA* Ready, int t_sim, int* p_aux_q, int quantum){
  if (cpu ->estado != 0){
    //Si la cpu esta ocupada...
    if (*p_aux_q == quantum && cpu ->process->List_rafagas->burst[0] ->CPU_Bust > 0){
          // Si es hora de interrumpir...
          cpu -> process -> t_rupcion++;
          cpu -> process -> estado = 1; //ready
          Proceso* new_process = clonar_proceso(cpu ->process);
          destroyProcessCPU(cpu);
          cpu -> estado = 0;
          ingresar_in_cola(Ready, new_process);
          //printf("PROCESO %s INTERRUMPIDO en t=%i\n", new_process->name, t_sim);
        }
    else{
      if(cpu ->process->List_rafagas->burst[0] ->CPU_Bust==0 && cpu ->process->List_rafagas->largo==1){
      // Si debo mandar un proceso a Finish...
        if (*p_aux_q == quantum){
          // interrupción por enunciado
          cpu -> process -> t_rupcion++;
        }
        *p_aux_q = 0;
        cpu -> process -> t_f = t_sim;
        cpu -> process -> estado = 4; //finish
        Proceso* new_process = clonar_proceso(cpu ->process);
        //libero cpu
        destroyProcessCPU(cpu);
        cpu -> estado = 0;
        ingresar_in_cola(Finish, new_process);
        //printf("PROCESO %s TERMINO en t=%i\n", new_process->name, new_process->t_f);
      }
      else{
        if(cpu ->process->List_rafagas->burst[0] ->CPU_Bust==0 && cpu ->process->List_rafagas->largo >1){
          //Considero que no existe interrupcion pasando directamente de cpu a waiting
          
          //Si me equivoco descomentar lo siguiente
          if (*p_aux_q == quantum){
            cpu -> process -> t_rupcion++;
          }
          // Deberia funcionar todo con esto
          
          // ...
          *p_aux_q = 0;
          // Si debo mandar un proceso a Waiting...  
          // proceso debe ir a waiting
          // Aqui no borro aun la rafaga porque me queda el waiting
          Proceso* new_process = clonar_proceso(cpu ->process);
          //libero cpu
          new_process ->estado = 3; //waiting
          //printf("PROCESO %s SALE de la CPU en t=%i\n",new_process->name,t_sim);
          destroyProcessCPU(cpu);
          cpu -> estado = 0;
          ingresar_in_cola(Waiting, new_process);
        }
      }
    }
  }
}

void actualizar_cpu(CPU* cpu, COLA* Finish, COLA* Waiting,int t_sim){
  /*
  - En CPU: si CPU-Bust es 0 entonces reviso el largo de las rafagas.
  Si este largo == 1 entonces el proceso pasa a estar en estado finish. y registro t de salida
  Si este largo > 1 entonces el proceso debe tener un waiting mode y 
    se saca de la CPU para entrar en la cola waiting.
  */
  if(cpu->estado!=0){
    if(cpu ->process->List_rafagas->burst[0] ->CPU_Bust==0 && cpu ->process->List_rafagas->largo==1){
      // el proceso debe irse a finish!
      cpu -> process -> t_f = t_sim;
      cpu -> process -> estado = 4; //finish
      Proceso* new_process = clonar_proceso(cpu ->process);
      //libero cpu
      destroyProcessCPU(cpu);
      cpu -> estado = 0;
      ingresar_in_cola(Finish, new_process);
      //printf("PROCESO %s TERMINO en t=%i\n", new_process->name, new_process->t_f);
    }
    else{
      if(cpu ->process->List_rafagas->burst[0] ->CPU_Bust==0 && cpu ->process->List_rafagas->largo >1){
        // proceso debe ir a waiting
        // Aqui no borro aun la rafaga porque me queda el waiting
        Proceso* new_process = clonar_proceso(cpu ->process);
        //libero cpu
        new_process ->estado = 3; //waiting
        //printf("PROCESO %s SALE de la CPU en t=%i\n",new_process->name,t_sim);
        destroyProcessCPU(cpu);
        cpu -> estado = 0;
        ingresar_in_cola(Waiting, new_process);
        
      }
    }
  }
}
bool hay_readys_in_waiting(COLA* waiting){
  int total = waiting->largo;
  for(int p = 0; p < total; p++){
    if (waiting -> process[p] ->estado == 1){
      return true;
    }
  }
  return false;
}

void actualizar_waiting(COLA* Waiting,COLA* Ready, int t_sim){
  // En waiting: si IO-Bust es 0 entonces borro la rafaga de la lista de rafagas del proceso
  //  y paso el proceso a ready
  if(Waiting->largo > 0){
    if(Waiting->largo == 1){
      //Si solo tengo un proceso en waiting
      if (Waiting->process[0]->List_rafagas->burst[0]->IO_Bust==0){
        Proceso* new_process = clonar_proceso(Waiting->process[0]);
        laserRafagas(new_process);
        new_process ->estado = 1; //Ready
        ingresar_in_cola(Ready, new_process);
        laser_cola(Waiting, 0);
        //printf("PROCESO %s SALE de WAITING en t=%i\n",new_process->name,t_sim);
      }
    }
    else{
      // Si hay más de un proceso en waiting
      //printf("En mantenimiento, largo %i\n",Waiting->largo);
      int total = Waiting->largo;
      
      for(int w = 0; w < total; w++){
        if(Waiting->process[w]->List_rafagas->burst[0]->IO_Bust==0){
      
          Proceso* new_process = clonar_proceso(Waiting->process[w]);
      
          laserRafagas(new_process);
          
          new_process ->estado = 1; //Ready
          ingresar_in_cola(Ready, new_process);
          Waiting -> process[w]->estado = 1;
          //printf("PROCESO %s SALE de WAITING en t=%i\n",new_process->name,t_sim);
        }
      }
      
      while(hay_readys_in_waiting(Waiting)){
      
        for(int w = 0; w < Waiting->largo; w++){
          if(Waiting->process[w]->estado == 1){
            laser_cola(Waiting,w);
            break;
          }
        }
      }
      
      /*
      int total = Waiting->largo;
      for(int w = total-1; w >= 0; w--){
        if(Waiting->process[w]->List_rafagas->burst[w]->IO_Bust==0){
          Proceso* new_process = clonar_proceso(Waiting->process[w]);
          laserRafagas(new_process);
          new_process ->estado = 1; //Ready
          ingresar_in_cola(Ready, new_process);
          laser_cola(Waiting, w);
          printf("PROCESO %s SALE de WAITING en t=%i\n",new_process->name,t_sim);
        }    
      }
      */
    }
  }
}