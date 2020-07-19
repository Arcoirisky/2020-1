// Indica que este archivo solo se debe incluir una vez en la compilación
#pragma once
//#include "structs.h"
// Declaramos las funciones asociadas

/** F al titanic */
int elegir_puerta(Titanic* terminales, int term, int prioridad, int* pos);
int cantidad_ente(Cola* cola, int prioridad);
int posicion_nueva(int prioridad, int* cola);
void insertar_cola(Puerta** door,int eleccion_puerta,Persona* persona_nueva, int posicion);
void imprimir_cola(Cola* cola);
/** Inserta un nuevo elemento al final de la lista */
//void list_append(Cola* list, int value);
/** Imprime todos los elementos de la lista */
//void list_print(Cola* list);
/** Libera todos los recursos asociados a esta lista */
//void list_destroy(Cola* list);
