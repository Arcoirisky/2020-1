// Indica que este archivo solo se debe incluir una vez en la compilación
#pragma once
void ingresar_in_cola(COLA* mod_cola, Proceso* new_process);
void laser_idle(IDLE* NoCreados, int index);
void laser_cola(COLA* cola, int index);
void destroy_Proceso_COLA(COLA* cola, int index);
void destroy_Proceso_IDLE(IDLE* NoCreados, int index);
Proceso* crear_proceso(IDLE* NoCreados, COLA* ready,int t0);