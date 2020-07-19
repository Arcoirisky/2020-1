
#include "ingreso_terminal.h"

int elegir_puerta(Titanic* terminales, int term, int prioridad)
{
  
  int opciones = terminales -> station[term] -> lista -> largo;
  int Cant[opciones][3];
  
  int puerta = 0;
  while(puerta < opciones){
    // identifico cuanta gente de cada tipo existe en cada puerta del terminal elegido
    
    int Cantidad[3] = {terminales -> station[term] -> lista -> door[puerta] -> ninos -> largo,
                      terminales -> station[term] -> lista -> door[puerta] -> adultos -> largo,
                      terminales -> station[term] -> lista -> door[puerta] -> robots -> largo
                      };
    for(int op = 0; op < 3; op++){ //3 es por los 3 tipos
      Cant[puerta][op] = Cantidad[op]; 
      //Cant[Puerta][ninos,adultos,robots]
    }
    puerta++;
  }
  int comparo;
  int menor;
  for(int doorr=0; doorr<opciones;doorr++){
    if (terminales -> station[term] -> lista -> door[doorr] -> estado == 0){
      comparo = posicion_nueva(prioridad, Cant[doorr]);
      menor = doorr;
      break;
    }
  }
  
  for(int i = 0; i < opciones; i++){
    if (terminales -> station[term] -> lista -> door[i] -> estado == 0){
      if(comparo > posicion_nueva(prioridad, Cant[i])){
        menor = i;
        comparo = posicion_nueva(prioridad, Cant[menor]);
      }
    }
  } 
  return menor;
};

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

void insertar_cola(Cola* mod_cola,Persona* persona_nueva){
  
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

void ingresar_ente(int passenger_id, int priority, Titanic* terminales,int terminal){
  Persona* persona_nueva = persona_init(passenger_id, priority);
  int eleccion_puerta = elegir_puerta(terminales, terminal, priority);
  
  if (priority == 0){
    Cola* mod_cola = (terminales -> station[terminal] -> lista -> door[eleccion_puerta] -> ninos);
    insertar_cola(mod_cola, persona_nueva);
  }
  else if (priority == 1){
    Cola* mod_cola = (terminales -> station[terminal] -> lista -> door[eleccion_puerta] -> adultos);
    insertar_cola(mod_cola, persona_nueva);
  }
  else if (priority == 2){
    Cola* mod_cola = (terminales -> station[terminal] -> lista -> door[eleccion_puerta] -> robots);
    insertar_cola(mod_cola, persona_nueva);
  }
      
}

void imprimir_cola(Cola* cola){
  //printf("imprimir cola\n");
  for(int cont=0;cont < cola -> largo; cont++){
    printf("%i\n", cola -> people[cont] -> id_persona);
  }
  //printf("\n -------------------- \n");
}
































