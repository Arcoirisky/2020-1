// Indica que este archivo solo se debe incluir una vez en la compilación
#pragma once
void sacar_proceso_ready_to_cpu(COLA* ready, CPU* cpu, int t_sim);
int elegir_continua(COLA* ready);
int sumaBusts(Proceso* process);