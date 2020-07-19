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

Puerta* puerta_init(int id){
  Puerta* puerta_nueva = malloc(sizeof(Puerta));
    puerta_nueva -> id_puerta = id;
    puerta_nueva -> estado = 0;
    puerta_nueva -> countPods = 0;
    
    for(int i = 0;i<3;i++){
    Cola* cola_nueva = malloc(sizeof(Cola));
    cola_nueva -> largo = 0; //para asegurarme, cualquier cosa la agrandare de a 5 mas adelante
    cola_nueva -> capacidad = 5;
    cola_nueva -> people = malloc(sizeof(Cola*)*(cola_nueva -> capacidad));
    if (i == 0){
      puerta_nueva -> ninos = cola_nueva;
    }
    else if(i ==1){
      puerta_nueva -> adultos = cola_nueva;
    }
    else if (i==2){
      puerta_nueva -> robots = cola_nueva;
    }
    }
  return puerta_nueva;
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
    Puerta* puerta_nueva = puerta_init(puer);
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
      // NINOS
      for(int persona = 0; persona < terminales -> station[i] -> lista -> door[puerta] -> ninos -> largo; persona++)
      {
        free(terminales -> station[i] -> lista -> door[puerta] -> ninos -> people[persona]); //libero a todas las personas de una fila
      }
      free(terminales -> station[i] -> lista -> door[puerta] -> ninos -> people); //libero la fila de una cola
      free(terminales -> station[i] -> lista -> door[puerta] -> ninos); //libero la cola de una
      // ADULTOS
      for(int persona = 0; persona < terminales -> station[i] -> lista -> door[puerta] -> adultos -> largo; persona++)
      {
        free(terminales -> station[i] -> lista -> door[puerta] -> adultos -> people[persona]); //libero a todas las personas de una fila
      }
      free(terminales -> station[i] -> lista -> door[puerta] -> adultos -> people); //libero la fila de una cola
      free(terminales -> station[i] -> lista -> door[puerta] -> adultos); //libero la cola de una
      //ROBOTS
      for(int persona = 0; persona < terminales -> station[i] -> lista -> door[puerta] -> robots -> largo; persona++)
      {
        free(terminales -> station[i] -> lista -> door[puerta] -> robots -> people[persona]); //libero a todas las personas de una fila
      }
      free(terminales -> station[i] -> lista -> door[puerta] -> robots -> people); //libero la fila de una cola
      free(terminales -> station[i] -> lista -> door[puerta] -> robots); //libero la cola de una
      // PUERTA
      free(terminales -> station[i] -> lista -> door[puerta]); //libero todas las puertas de 1 stage
    }
    free(terminales -> station[i] -> lista -> door); //libero el stage de puertas
    free(terminales -> station[i] -> lista); //libero la lista de stages por estacion
    free(terminales -> station[i]); //
  }
  free(terminales -> station);
  free(terminales);  
  
}
