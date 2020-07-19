#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "structs.h"
#include "ingreso_terminal.h"
#include "laser.h"
#include "clausura.h"
/** Retorna true si ambos strings son iguales */
bool string_equals(char* string1, char* string2)
{
  return !strcmp(string1, string2);
}

void simulate(char* filename)
{
  // Abrimos el archivo
  FILE* file = fopen(filename, "r");
  
  // Leemos la cantidad de terminales
  int terminal_count;
  fscanf(file, "%i", &terminal_count);

  // TODO: Inicializar estructura principal
  // esto esta en structs.h y .c
  Titanic* terminales = titanic_init(terminal_count);

  for (int term = 0; term < terminal_count; term++)
  {
    int gate_count; //- solo me dice la cantidad de puertas
    fscanf(file, "%i", &gate_count); //- aqui es donde saca el valor del txt

    Terminal* terminal_nuevo = terminal_init(term, gate_count);
    terminales -> station[term] = terminal_nuevo;
  }
  
  char command[32];

  while(true)
  {
    
    
    fscanf(file, "%s", command);

    if(string_equals(command, "END"))
    {
      break;
    }
    else if(string_equals(command, "INGRESO")) // Done
    {
      int terminal, passenger_id, priority;

      fscanf(file, "%i %i %i", &terminal, &passenger_id, &priority);
      //printf("ingreso al terminal %i\n",terminal);
      // TODO: Procesar ingreso
      ingresar_ente(passenger_id, priority, terminales,terminal);
      
    }
    else if(string_equals(command, "ABORDAJE")) // Done
    {
      int terminal, gate;
      fscanf(file, "%i %i", &terminal, &gate);
      //Debemos liberar a las primeras 8 personas.
      //printf("Inicio del abordaje en el terminal %i puerta %i\n", terminal, gate);

      // TODO: Procesar abordaje

      printf("POD %i %i %i LOG\n", terminal, gate, terminales -> station[terminal] -> lista -> door[gate] -> countPods);
      Puerta* puerta = terminales -> station[terminal] -> lista -> door[gate];
      int n_liberados = 0;
      while (n_liberados < 8){
        if (puerta -> ninos -> largo > 0){
          printf("%i\n",puerta -> ninos -> people[0] -> id_persona);
          matarPersona(puerta->ninos,0);
        }
        else if(puerta -> adultos -> largo > 0){
          printf("%i\n",puerta -> adultos -> people[0] -> id_persona);
          matarPersona(puerta->adultos,0);
        }
        else if(puerta -> robots -> largo > 0){
          printf("%i\n",puerta -> robots -> people[0] -> id_persona);
          matarPersona(puerta -> robots,0);
        }
        n_liberados++;
      }
      puerta -> countPods++;
    }

    else if(string_equals(command, "CIERRE"))
    {
      int terminal, gate;
      fscanf(file, "%i %i", &terminal, &gate);
      // TODO: Procesar cierre de puerta
      //printf("Cierre de puerta %i en terminal %i\n", gate, terminal);
      Puerta* puerta = terminales -> station[terminal] -> lista -> door[gate];
      puerta -> estado = 2;
      // Movamos a los ninos
      for(int n = 0; n < puerta -> ninos -> largo; n++){
        Persona* persona_nueva = puerta -> ninos -> people[n];
        int priorityN = persona_nueva -> tipo;
        int passenger_idN = persona_nueva -> id_persona;
        ingresar_ente(passenger_idN, priorityN, terminales, terminal);
        free(puerta -> ninos -> people[n]);
      }
      puerta -> ninos ->largo = 0;
      // Movamos a los adultos
      
      for(int a = 0; a < puerta -> adultos ->largo; a++){
        Persona* persona_nueva = puerta -> adultos -> people[a];
        int priorityA = persona_nueva -> tipo;
        int passenger_idA = persona_nueva -> id_persona;
        ingresar_ente(passenger_idA, priorityA, terminales,terminal);
        free(puerta -> adultos -> people[a]);
      }
      puerta -> adultos ->largo = 0;
      
      // Movamos a los robots
      for(int r = 0; r < puerta -> robots ->largo; r++){
        Persona* persona_nueva = puerta -> robots -> people[r];
        int priorityR = persona_nueva -> tipo;
        int passenger_idR = persona_nueva -> id_persona;
        ingresar_ente(passenger_idR, priorityR, terminales,terminal);
        free(puerta -> robots -> people[r]);
      }
      puerta -> robots -> largo = 0;
      
      
      //printf("Cierre de puerta %i en terminal %i\n", gate, terminal);
      
    }
    else if(string_equals(command, "CLAUSURA"))
    { 
      // TODO: Procesar clausura de terminal
      int term_out, term_in;
      fscanf(file, "%i %i", &term_out, &term_in);

      terminales -> station[term_out] -> estado = 2;
      meter_entes(terminales, term_out, term_in);

      //printf("Clausura de terminal %i, todos los pasajeros proceder al terminal %i\n", term_out, term_in);
    }
    else if(string_equals(command, "LASER")) // Done
    { 
      int terminal, gate, index;
      fscanf(file, "%i %i %i", &terminal, &gate, &index);
      //printf("Laser perdido impacta a la persona en indice %i de la fila para la puerta %i del terminal %i\n", index, gate, index);
      // primero necesito encontrar la cola correcta
      int n = terminales -> station[terminal] -> lista -> door[gate] -> ninos -> largo;
      int a = terminales -> station[terminal] -> lista -> door[gate] -> adultos -> largo;
      int r = terminales -> station[terminal] -> lista -> door[gate] -> robots -> largo;
      
      if (index < n){
        // esta persona esta dentro de los ninos
        //printf("muere un niño\n");
        Cola* mod_cola = terminales -> station[terminal] -> lista -> door[gate] -> ninos;
        matarPersona(mod_cola, index);
      }
      else if( index < n + a){
        // esta persona esta dentro de los adultos
        //printf("muere un adulto\n");
        Cola* mod_cola = terminales -> station[terminal] -> lista -> door[gate] -> adultos;
        matarPersona(mod_cola, index-n);
      }
      else if( index < n + a + r){
        // esta persona esta dentro de los robots
        //printf("muere un robot\n");
        Cola* mod_cola = terminales -> station[terminal] -> lista -> door[gate] -> robots;
        matarPersona(mod_cola, index-n-a);
      }
    }
  }

  fclose(file);
  printf("TITANIC LOG\n");
  // TODO: Liberar recursos e imprimir los pasajeros que siguen a bordo
  
  for(int terminal=0; terminal < terminales -> largo; terminal++){
    if(terminales -> station[terminal] ->estado == 0){
      printf("TERMINAL %i\n",terminal);
      for(int puerta = 0; puerta < terminales -> station[terminal] -> lista -> largo; puerta++){
        if(terminales -> station[terminal] -> lista -> door[puerta] -> estado == 0){
          int n = terminales -> station[terminal] -> lista -> door[puerta] -> ninos -> largo;
          int a = terminales -> station[terminal] -> lista -> door[puerta] -> adultos -> largo;
          int r = terminales -> station[terminal] -> lista -> door[puerta] -> robots -> largo;
          printf("GATE %i: %i\n", puerta, n+r+a);
          imprimir_cola(terminales -> station[terminal] -> lista -> door[puerta] -> ninos);
          imprimir_cola(terminales -> station[terminal] -> lista -> door[puerta] -> adultos);
          imprimir_cola(terminales -> station[terminal] -> lista -> door[puerta] -> robots);
        }
      }
    }
  }
  
  
  /** Libera todos los recursos asociados a esta lista */
  //- Liberar terminal
  glaciar(terminales);

  printf("END LOG\n");
}

int main(int argc, char *argv[])
{
  // Este programa recibe dos parámetros:
  //  argv[0] = el programa en sí
  //  argv[1] = la ruta al archivo de input
  if (argc != 2)
  {
    printf("Cantidad de parámetros incorrecta\n");
    printf("Uso correcto: %s PATH_TO_INPUT\n", argv[0]);
    return 1;
  }

  simulate(argv[1]);

  return 0;
}
