#include "structs.h"
#include "ingreso_terminal.h"
#include <stdlib.h>
#include <stdio.h>

int elegir_puerta(Titanic* terminales, int term, int prioridad, int* pos)
{
  int opciones = terminales -> station[term] -> lista -> largo;
  int Cant[opciones][3];
  
  for(int puerta = 0; puerta < opciones; puerta++){
    // identifico cuanta gente de cada tipo existe en cada puerta del terminal elegido
    int Cantidad[3] = {cantidad_ente(terminales -> station[term] -> lista -> door[puerta] -> cola, 0),
                       cantidad_ente(terminales -> station[term] -> lista -> door[puerta] -> cola, 1),
                       cantidad_ente(terminales -> station[term] -> lista -> door[puerta] -> cola, 2)
                      };
    for(int op = 0; op < 3; op++){ //3 es por los 3 tipos
      Cant[puerta][op] = Cantidad[op]; 
      //Cant[Puerta][ninos,adultos,robots]
    }
    
  }
  
  int comparo = posicion_nueva(prioridad, Cant[0]);//Cant[0][prioridad];
  int menor = 0;
  int i = 0;
  while(i < opciones){
    //printf("mi puerta i: %i, menor: %i, comparo: %i, estado: %i\n",i,menor,comparo, terminales -> station[term] -> lista -> door[i]->estado);
    if(terminales -> station[term] -> lista -> door[i] -> estado == 1){
      i++;
      menor = i;
      comparo = posicion_nueva(prioridad, Cant[menor]); 
    }
    else{
      if(comparo > posicion_nueva(prioridad, Cant[i])){
        menor = i;
        comparo = posicion_nueva(prioridad, Cant[menor]);
      }
      else{
      }
    }
    i++;
  }
  *pos = comparo;
  //printf("posicion nueva %i\n", *pos);
  return menor;
};

int cantidad_ente(Cola* cola, int prioridad){
  int count = 0;
  for(int persona=0; persona < cola -> largo; persona++){
    if(cola -> people[persona] -> tipo == prioridad){
    count += 1;
    }
    else{
    }
  }
  return count;
}

int posicion_nueva(int prioridad, int* cola){
  if (prioridad == 0)
  {
    return cola[0];
  }
  else if (prioridad == 1)
  {
    return cola[0] + cola[1];
  }
  else
  {
    return cola[0] + cola[1] + cola[2];
  }
}

void insertar_cola(Puerta** door,int eleccion_puerta,Persona* persona_nueva, int posicion){
  int tengo = door[eleccion_puerta] -> cola -> largo;
  int capacidad = door[eleccion_puerta] -> cola -> capacidad;
  if (tengo == 0){
        door[eleccion_puerta] -> cola -> people[posicion] = persona_nueva;
        door[eleccion_puerta] -> cola -> largo += 1;
  }
  else if (tengo < capacidad){
    if (posicion < tengo){ // intento guardar en una posición ya ocupada
      // Creo cola_aux standar
      Cola* cola_aux = malloc(sizeof(Cola));
      cola_aux -> capacidad = door[eleccion_puerta] -> cola -> capacidad;
      cola_aux -> largo = tengo + 1;
      cola_aux -> people = malloc(sizeof(Cola*)*(cola_aux -> capacidad));
      // Guardo todas las personas antes de la posición elegida
      for(int i=0; i < posicion;i++){
        cola_aux -> people[i] = door[eleccion_puerta] -> cola -> people[i];
      }
      // Guardo la persona en la posición correspondiente
      cola_aux -> people[posicion] = persona_nueva;
      // Guardo el resto de gente de la cola en el aux
      for(int j=posicion; j < tengo; j++){
        cola_aux -> people[j + 1] = door[eleccion_puerta] -> cola -> people[j];
      }
      // libero cola sin liberar a la gente, porque la tengo referenciada en la otra cola
      free(door[eleccion_puerta] -> cola -> people);
      free(door[eleccion_puerta] -> cola);
      door[eleccion_puerta] -> cola = cola_aux;
     }
    else{ // posicion >= tengo dentro de la capacidad de la cola
      door[eleccion_puerta] -> cola -> people[posicion] = persona_nueva;
      door[eleccion_puerta] -> cola -> largo += 1;
      
    }
  }
  else{
    //Caso tengo >= capacidad aka necesito mas espacio en mi cola para agregarlos
    Cola* cola_aux = malloc(sizeof(Cola));
    cola_aux -> capacidad = (door[eleccion_puerta] -> cola -> capacidad) + 20;
    cola_aux -> largo = tengo + 1;
    cola_aux -> people = malloc(sizeof(Cola*)*((door[eleccion_puerta] -> cola -> capacidad) + 5));
    if (posicion < tengo){
      //guardo lo que tenia antes de la posicion
      for(int i=0; i < posicion;i++){
        cola_aux -> people[i] = door[eleccion_puerta] -> cola -> people[i];
      }
      // Guardo la persona en la posición correspondiente
      cola_aux -> people[posicion] = persona_nueva;
      // Guardo el resto de gente de la cola en el aux
      for(int j=posicion; j < tengo; j++){
      cola_aux -> people[j + 1] = door[eleccion_puerta] -> cola -> people[j];
      }
    //libero la cola antigua para agregar la nueva con mas 5 espacios
      free(door[eleccion_puerta] -> cola -> people);
      free(door[eleccion_puerta] -> cola);
      door[eleccion_puerta] -> cola = cola_aux;
    }
    else{//caso que la persona nueva va al final
      //guardo lo que tenia antes de la posicion
      for(int i=0; i < posicion;i++){
        cola_aux -> people[i] = door[eleccion_puerta] -> cola -> people[i];
      }
      //guardo a la persona nueva
      cola_aux -> people[posicion] = persona_nueva;
      //reemplazo la lista sin espacio por la nueva con +5
      free(door[eleccion_puerta] -> cola -> people);
      free(door[eleccion_puerta] -> cola);
      
      door[eleccion_puerta] -> cola = cola_aux;
    }
    
  }
  //imprimir_cola(door[eleccion_puerta] -> cola);
}

void imprimir_cola(Cola* cola){
  //printf("imprimir cola\n");
  for(int cont=0;cont < cola -> largo; cont++){
    printf("%i\n", cola -> people[cont] -> id_persona);
  }
  //printf("\n -------------------- \n");
}
































