// Indica que este archivo solo se debe incluir una vez en la compilación
#pragma once

struct Rafaga;
struct Rafaga
{
  int CPU_Bust;
  int IO_Bust;
};
typedef struct Rafaga Rafaga;

struct ListaRafagas;
struct ListaRafagas
{
  Rafaga** burst;
  int largo;
};
typedef struct ListaRafagas Lista;

struct Proceso;
struct Proceso
{
  char name[32];
  int estado;

  int t_0;
  int t_cpu;
  int t_f;
  int t_rupcion;
  int turnos_cpu;
  int WaitingTime;
  Lista* List_rafagas;
};
typedef struct Proceso Proceso;

struct IDLEProcess;
struct IDLEProcess
{
  int largo;
  int total;
  Proceso** process;
};
typedef struct IDLEProcess IDLE;

struct COLAReady;
struct COLAReady
{
  int largo;
  int capacidad;
  Proceso** process;
};
typedef struct COLAReady COLA;

struct CPU;
struct CPU
{
  Proceso* process;
  int estado;
};
typedef struct CPU CPU;


IDLE* NoCreados_Init(char* input);
COLA* cola_init();
CPU* cpu_init();
Proceso* clonar_proceso(Proceso* old_proceso);
void destroyIDLE(IDLE* NoCreados);
void destroyCola(COLA* cola);
void destroyCPU(CPU* cpu);
void destroyProcessCPU(CPU* cpu);