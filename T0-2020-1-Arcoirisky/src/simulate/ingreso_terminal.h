// Indica que este archivo solo se debe incluir una vez en la compilación
#pragma once
#include "structs.h"
#include <stdlib.h>
#include <stdio.h>

//#include "structs.h"
// Declaramos las funciones asociadas

/** F al titanic */
void ingresar_ente(int passenger_id, int priority, Titanic* terminales,int terminal);
int elegir_puerta(Titanic* terminales, int term, int prioridad);
int posicion_nueva(int prioridad, int* cola);
void insertar_cola(Cola* mod_cola,Persona* persona_nueva);
void imprimir_cola(Cola* cola);
