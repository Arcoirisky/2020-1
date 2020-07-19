// Indica que este archivo solo se debe incluir una vez en la compilación
#pragma once
#include "structs.h"

void actualizar_t_ready(COLA* ready);
void actualizar_t_waiting(COLA* waiting);
void actualizar_t_cpu(CPU* cpu);
void actualizar_cpu_2(CPU* cpu, COLA* Finish, COLA* Waiting, COLA* Ready, int t_sim, int* p_aux_q, int quantum);
void actualizar_cpu(CPU* cpu, COLA* Finish, COLA* Waiting,int t_sim);
void actualizar_waiting(COLA* Waiting,COLA* Ready, int t_sim);