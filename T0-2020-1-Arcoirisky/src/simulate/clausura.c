#include "structs.h"
#include "ingreso_terminal.h"
#include "clausura.h"
#include "laser.h"
#include <stdlib.h>
#include <stdio.h>

Persona* sacarPersonaPuerta(Puerta* puerta, int index, int n, int a, int r){
  if (index < n){
    return puerta -> ninos -> people[index];
  }
  else if( index < n + a){
    return puerta -> adultos -> people[index-n];
  }
  return puerta -> robots -> people[index-n-a];
}

bool ExisteGenteEn(Puerta* puerta){
  if (puerta -> ninos -> largo > 0){
    return true;
  }
  else if (puerta -> adultos -> largo > 0){
    return true;
  }
  else if (puerta -> robots -> largo > 0){
    return true;
  }
  return false;
}

bool quedan_entes2(Lista* Puertas, int n){
  for(int i = 0; i< n ; i++){
    if(Puertas -> door[i] -> estado == 0 && ExisteGenteEn(Puertas -> door[i])){
      return true;
    }
  }
  return false;
}

void insertar_cola_aux(Cola* mod_cola, int passenger_id, int priority){
  Persona* persona_nueva = persona_init(passenger_id, priority);
  int tengo = mod_cola -> largo;
  int capacidad = mod_cola -> capacidad;
  if (tengo < capacidad){
        mod_cola -> people[tengo] = persona_nueva;
        mod_cola -> largo += 1;
  } 
  else{ //no tengo más espacio:
    
      mod_cola -> people = realloc(mod_cola -> people,sizeof(Cola*)*((mod_cola -> capacidad)+30));
      mod_cola -> capacidad+= 30;
      insertar_cola(mod_cola, persona_nueva);
      //mod_cola -> people[tengo] = persona_nueva;
      //mod_cola -> largo = tengo + 1;
      
    }
  
}

void meter_entes(Titanic* terminales, int ter1, int ter2){//, int tipo){
  // terminal out = ter1
  Lista* P_salida = terminales -> station[ter1] -> lista;
  int total_puertas = P_salida -> largo;

  int Cant[total_puertas][3];
  int puer = 0;
  while(puer < total_puertas){
    // identifico cuanta gente de cada tipo existe en cada puerta
    int Cantidad[3] = {P_salida -> door[puer] -> ninos -> largo,
                      P_salida -> door[puer] -> adultos -> largo,
                      P_salida -> door[puer] -> robots -> largo
                      };
    for(int op = 0; op < 3; op++){ //3 es por los 3 tipos
      Cant[puer][op] = Cantidad[op]; 
      //Cant[Puerta][ninos,adultos,robots]
    }
    puer++;
  }
  int index[total_puertas];
  for(int j = 0; j<total_puertas; j++){
    index[j] = 0;
  }
  //printf("hasta aqui todo bien\n");
  Puerta* puerta_aux = puerta_init(666);

  while(quedan_entes2(P_salida,total_puertas)){
    for(int i=0;i<total_puertas;i++){
      Puerta* puerta = P_salida -> door[i];
      if (ExisteGenteEn(puerta)){
        int n = Cant[i][0];
        int a = Cant[i][1];
        int r = Cant[i][2];
        //printf("n: %i, a:%i, r:%i, index:%i\n",n,a,r,index[i]);
        Persona* persona_nueva = sacarPersonaPuerta(puerta, index[i], n,a,r);
        int priority = persona_nueva -> tipo;
        int passenger_id = persona_nueva -> id_persona;
        //printf("obtuve a la persona %i\n", passenger_id);
        /*
        if(passenger_id == 3863521 || passenger_id == 64811999 || passenger_id == 41004511){
          printf("TERMINAR1 %i -> TERMINAL2 %i\n",ter1,ter2);
          printf("PASAJERO %i PUERTA out %i posicion %i\n", passenger_id,i,index[i]);
          printf("-------------\n");
        }
        */
        //ingresar_ente(passenger_id, priority, terminales, ter2);
        
        if (priority == 0){
          insertar_cola_aux(puerta_aux -> ninos, passenger_id, priority);
          free(puerta -> ninos -> people[index[i]]);
          puerta -> ninos -> largo--;
        }
        else if (priority == 1){
          insertar_cola_aux(puerta_aux -> adultos, passenger_id, priority);
          free(puerta -> adultos -> people[index[i]-n]);
          puerta -> adultos -> largo--;
        }
        else if (priority == 2){
          insertar_cola_aux(puerta_aux -> robots, passenger_id, priority);
          free(puerta -> robots -> people[index[i]-n-a]);
          puerta -> robots -> largo--;
        }
        index[i]++;
      }
    }
  }
  //printf("ahora deberia meter a los entes\n");
  for(int n = 0; n < puerta_aux -> ninos -> largo; n++){
    Persona* persona_nueva = puerta_aux -> ninos -> people[n];
    int priorityN = persona_nueva -> tipo;
    int passenger_idN = persona_nueva -> id_persona;
    ingresar_ente(passenger_idN, priorityN, terminales, ter2);
    free(puerta_aux -> ninos -> people[n]);
  }
  puerta_aux -> ninos ->largo = 0;
  // Movamos a los adultos
  
  for(int a = 0; a < puerta_aux -> adultos ->largo; a++){
    Persona* persona_nueva = puerta_aux -> adultos -> people[a];
    int priorityA = persona_nueva -> tipo;
    int passenger_idA = persona_nueva -> id_persona;
    ingresar_ente(passenger_idA, priorityA, terminales,ter2);
    free(puerta_aux -> adultos -> people[a]);
  }
  puerta_aux -> adultos ->largo = 0;
  
  // Movamos a los robots
  for(int r = 0; r < puerta_aux -> robots ->largo; r++){
    Persona* persona_nueva = puerta_aux -> robots -> people[r];
    int priorityR = persona_nueva -> tipo;
    int passenger_idR = persona_nueva -> id_persona;
    ingresar_ente(passenger_idR, priorityR, terminales,ter2);
    free(puerta_aux -> robots -> people[r]);
  }
  puerta_aux -> robots -> largo = 0;
  free(puerta_aux -> robots -> people);
  free(puerta_aux -> adultos -> people);
  free(puerta_aux -> ninos -> people);
  free(puerta_aux -> robots);
  free(puerta_aux -> adultos);
  free(puerta_aux -> ninos);
  free(puerta_aux);
}



























