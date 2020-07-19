// Indica que este archivo solo se debe incluir una vez en la compilación
#pragma once
#include <stdbool.h>
void insertar_cola_aux(Cola* mod_cola, int passenger_id, int priority);
void meter_entes(Titanic* terminales, int ter1, int ter2);
bool quedan_entes2(Lista* Puertas, int n);
bool ExisteGenteEn(Puerta* puerta);
Persona* sacarPersonaPuerta(Puerta* puerta, int index, int n, int a, int r);