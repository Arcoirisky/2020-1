#include "structs.h"
#include <stdlib.h>
#include <stdio.h>


/** Inicializa un titanic con terminal_count terminales */
Titanic* titanic_init(int terminal_count)
{
  Titanic* terminales = malloc(sizeof(Titanic));
  terminales -> largo = terminal_count;
  terminales -> station = malloc(sizeof(Terminal*)*terminal_count);

  return terminales;
}

/** inicializa un terminal id term con una lista de gate_count puertas, cada una con su propia lista de cap 20 inicial*/
Terminal* terminal_init(int term, int gate_count)
{
  Terminal* terminal_nuevo = malloc(sizeof(Terminal));
  terminal_nuevo -> id_terminal = term;
  terminal_nuevo -> estado = 0;

  Lista* lista_nueva = malloc(sizeof(Lista)); //inicializo la lista
  lista_nueva -> largo = gate_count;
  lista_nueva -> door = malloc(sizeof(Puerta*)*gate_count);
    
  terminal_nuevo -> lista = lista_nueva;
  for(int puer = 0; puer < gate_count; puer++)
  {
    //Aqui estoy creando las N puertas
    Puerta* puerta_nueva = malloc(sizeof(Puerta));
    puerta_nueva -> id_puerta = puer;
    puerta_nueva -> estado = 0;
    puerta_nueva -> countPods = 0;
      
    Cola* cola_nueva = malloc(sizeof(Cola));
    cola_nueva -> largo = 0; //para asegurarme, cualquier cosa la agrandare de a 5 mas adelante
    cola_nueva -> capacidad = 30;
    cola_nueva -> people = malloc(sizeof(Cola*)*(cola_nueva -> capacidad));
    
    puerta_nueva -> cola = cola_nueva;

    terminal_nuevo -> lista -> door[puer] = puerta_nueva;
   }
return terminal_nuevo;
}

/** creo una persona id passenger_id y de tipo priority */
Persona* persona_init(int passenger_id, int priority)
{
  Persona* persona_nueva = malloc(sizeof(Persona));
  persona_nueva -> id_persona = passenger_id;
  persona_nueva -> tipo = priority;
  
  return persona_nueva;
}

/** Destroy Titanic */
void glaciar(Titanic* terminales)
{
  int terminal_count = terminales -> largo;
  for(int i=0; i < terminal_count; i++)
  {
    for (int puerta = 0; puerta < terminales -> station[i] -> lista -> largo; puerta++)
    {
      for(int persona = 0; persona < terminales -> station[i] -> lista -> door[puerta] -> cola -> largo; persona++)
      {
        free(terminales -> station[i] -> lista -> door[puerta] -> cola -> people[persona]); //libero a todas las personas de una fila
      }
      free(terminales -> station[i] -> lista -> door[puerta] -> cola -> people); //libero la fila de una cola
      free(terminales -> station[i] -> lista -> door[puerta] -> cola); //libero la cola de una
      
      free(terminales -> station[i] -> lista -> door[puerta]); //libero todas las puertas de 1 stage
    }
    free(terminales -> station[i] -> lista -> door); //libero el stage de puertas
    free(terminales -> station[i] -> lista); //libero la lista de stages por estacion
    free(terminales -> station[i]); //
  }
  free(terminales -> station);
  free(terminales);  
  
}
