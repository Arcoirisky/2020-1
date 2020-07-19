#include "structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void escribir_salida_np(COLA* Finish,char* output){
    FILE* file = fopen(output, "w");
    for(int elem = 0; elem < Finish->largo; elem++){
        // nombre_proceso, turnos_cpu, interrupciones, turnaraund, responseTime, waitingTime
        Proceso* process = Finish->process[elem];
        int turnaround = ((process->t_f)-(process->t_0))+1;
        int responseTime = process->t_cpu - (process->t_0);
        int waitingTime = process->WaitingTime;
        int cpu_turn = process -> turnos_cpu;
        fprintf(file, "%s, %i, 0, %i, %i, %i\n",process->name, cpu_turn,turnaround, responseTime, waitingTime);
    }
    fclose(file);

}

void escribir_salida_p(COLA* Finish,char* output){
    FILE* file = fopen(output, "w");
    for(int elem = 0; elem < Finish->largo; elem++){
        // nombre_proceso, turnos_cpu, interrupciones, turnaraund, responseTime, waitingTime
        Proceso* process = Finish->process[elem];
        int turnaround = ((process->t_f)-(process->t_0))+1;
        int responseTime = process->t_cpu - (process->t_0);
        int waitingTime = process->WaitingTime;
        int cpu_turn = process -> turnos_cpu;
        int interrupciones = process ->t_rupcion;
        fprintf(file, "%s, %i, %i, %i, %i, %i\n",process->name, cpu_turn,interrupciones, turnaround,responseTime, waitingTime);
    }
    fclose(file);

}
